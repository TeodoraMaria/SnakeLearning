#include "MultisnakeTabularTrainer.hpp"
#include "GameLogic/Game.h"
#include "State.h"
#include "ActionPickingUtils.h"
#include "QTabStudent.hpp"

#include "AI/QLearning/QTable.h"
#include "AI/HardCoded/SingleBot.hpp"

#include "GymEnv/SnakeStudent.hpp"
#include "GymEnv/StateObserver/GridObserver.hpp"
#include "GymEnv/StateExtractor.hpp"

#include "GameLogic/CellInterpreter/Basic3CellInterpreter.hpp"
#include "GameLogic/CellInterpreter/WallFoodBody.hpp"

#include "Utils/MathUtils.h"
#include "Utils/PrintUtils.h"
#include "Utils/MatrixUtils.h"

#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
#include <assert.h>

using namespace AI::QLearning;
using namespace GameLogic::CellInterpreter;
using namespace GymEnv::StateObserver;
using namespace GymEnv::Utils;

MultisnakeTabularTrainer::MultisnakeTabularTrainer(
	const GameOptions& gmOptions,
	const QOptions& qoptions
) :
	m_gmoptions(gmOptions),
	m_qoptions(qoptions),
	m_merseneTwister(std::random_device()())
{
}

void MultisnakeTabularTrainer::TryInitQField(QTable& qtable, const State key)
{
	if (qtable.find(key) == qtable.end())
	{
		qtable[key] = ::Utils::Matrix::MakeVector(
			IPlayer::possibleMoves.size(),
			[&]() { return m_qoptions.tabInitializer(m_merseneTwister); });
	}
}

IPlayer* MultisnakeTabularTrainer::Train()
{
	auto cellInterpreter1 = std::make_shared<WallFoodBody>();
	auto agent1 = std::make_shared<QTabStudent>(
		cellInterpreter1,
		new GridObserver(cellInterpreter1, 5, 5),
		[&]() { return m_qoptions.tabInitializer(m_merseneTwister); }
	);
	
	auto cellInterpreter2 = std::make_shared<WallFoodBody>();
	auto agent2 = std::make_shared<QTabStudent>(
		cellInterpreter2,
		new GridObserver(cellInterpreter2, 5, 5),
		[&]() { return m_qoptions.tabInitializer(m_merseneTwister); }
	);

	auto agents = std::vector<std::shared_ptr<QTabStudent>>(
	{
		agent1,
		agent2
	});
	
	auto players = std::vector<std::shared_ptr<IPlayer>>();
	for (auto agent : agents)
	{
		players.push_back(std::static_pointer_cast<IPlayer>(agent));
		agent->SetNextAction(SnakeMove::LEFT);
	}

	auto game = Game(m_gmoptions, players);

	for (const auto agent : agents)
	{
		agent->SetNoise(m_qoptions.maxRandActionChance);
	}
	
	for (auto episode = 0u; episode < m_qoptions.numEpisodes; episode++)
	{
		for (auto& agent : agents)
			agent->PrepareForNewEpisode();
		game.InitGame();
		
		const auto maxNbOfSteps = m_qoptions.maxNumSteps(episode);
		for (auto step = 0u; step < maxNbOfSteps; step++)
		{
			if (game.EveryoneIsDead())
				break;
			
			for (auto& agent : agents)
			{
				auto gmState = game.GetGameState();
				
				const auto snakeId = agent->GetSnakeNumber();
				if (!gmState.GetSnake(snakeId).IsAlive())
					continue;
				
				const State state = agent->ObserveState(gmState);
				const auto actionIndx = agent->PickAction(state, m_merseneTwister);

				assert(actionIndx < IPlayer::possibleMoves.size());
				const auto rawReward = game.MoveSnake(
					snakeId,
					IPlayer::possibleMoves[actionIndx]);
				
				auto reward = 0.0;
				if (rawReward < 0)
					reward += m_qoptions.dieReward(episode);
				else if (rawReward > 0)
					reward += m_qoptions.foodReward(episode);
				else
					reward += m_qoptions.stepReward(episode);
				agent->SetReward(agent->GetReward() + reward);

				State newState;
				double bestNextQ = 0;
				
				const auto isDone = (rawReward < 0);
				if (!isDone)
				{
					const auto gmState = game.GetGameState();
					newState = agent->ObserveState(gmState);
					bestNextQ = agent->GetBestQualityFromState(newState);
				}
				
				agent->UpdateQTab(
					state,
					bestNextQ,
					actionIndx,
					reward,
					m_qoptions.learningRate,
					m_qoptions.qDiscountFactor);
				
				if (rawReward > 0)
					agent->SetStepsWithoutFood(0);
				else if (agent->GetStepsWithoutFood() >= m_qoptions.maxStepsWithoutFood(episode))
					game.ForcefullyKillPlayer(snakeId);
				else
					agent->SetStepsWithoutFood(agent->GetStepsWithoutFood() + 1);
				
				agent->SetNoise(
					::Utils::Math::Lerp(
						agent->GetNoise(),
						m_qoptions.minRandActionChance,
						m_qoptions.randActionDecayFactor));
			}
			
			if (episode >= m_qoptions.numEpisodes - m_qoptions.lastNGamesToRender)
			{
				m_gmoptions.gameRenderer->Render(game.GetGameState());
				if (m_qoptions.milsToSleepBetweenFrames != 0)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(
						m_qoptions.milsToSleepBetweenFrames));
				}
			}
			
			game.RestockFood();
		}
		
		for (const auto& agent : agents)
		{
			printf(
				"End of episode: %d with a reward of %.2f."
				"Random action chance: %.2f\n",
				episode,
				agent->GetReward(),
				agent->GetNoise());
		}
	}
	return nullptr;
}
