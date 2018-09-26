#include "MultisnakeTabularTrainer.hpp"
#include "GameLogic/Game.h"
#include "State.h"
#include "ActionPickingUtils.h"
#include "QTabStudent.hpp"

#include "GameView/TermRenderer.hpp"

#include "AI/QLearning/QTable.h"
#include "AI/QLearning/TabularQJsonUtils.h"
#include "AI/HardCoded/SingleBot.hpp"

#include "GymEnv/SnakeStudent.hpp"
#include "GymEnv/StateObserver/GridObserver.hpp"
#include "GymEnv/StateObserver/FoodDirectionDecorator.hpp"
#include "GymEnv/StateExtractor.hpp"

#include "GameLogic/CellInterpreter/Basic3CellInterpreter.hpp"
#include "GameLogic/CellInterpreter/WallFoodBody.hpp"
#include "GameLogic/CellInterpreter/WallFoodEnemy.hpp"

#include "ConfigLoading/QTabStudentJson.h"

#include "Utils/MathUtils.h"
#include "Utils/PrintUtils.h"
#include "Utils/MatrixUtils.h"

#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
#include <json.hpp>
#include <assert.h>
#include <fstream>
#include <iomanip>

using namespace AI::QLearning;
using namespace GameLogic::CellInterpreter;
using namespace GymEnv::StateObserver;
using namespace GymEnv::Utils;

MultisnakeTabularTrainer::MultisnakeTabularTrainer(bool renderByYourself) :
	m_gmoptions(),
	m_qoptions(),
	m_merseneTwister(std::random_device()())
{
	auto& gmOptions = m_gmoptions;
	auto& qoptions = m_qoptions;
	
	gmOptions = GameOptions();
	{
		gmOptions.boardLength = 25;
		gmOptions.boardWidth = 25;
		gmOptions.numFoods = 10;
		gmOptions.initialSnakeSize = 3;
		
		if (!renderByYourself)
			gmOptions.gameRenderer = nullptr;
		else
		{
			gmOptions.gameRenderer = new GameView::TermRenderer();
	//		gmOptions.gameRenderer = new GameView::OpenGLRenderer(
	//			500, 500,
	//			gmOptions.boardLength, gmOptions.boardWidth);
		}
	}
	
	qoptions = AI::QLearning::QOptions();
	{
		qoptions.maxNumSteps = [&](int episode)
		{
			return 50000;// + (double)episode / qoptions.numEpisodes * 3000;
		};
		qoptions.qDiscountFactor = 0.9;
		qoptions.actionQualityEps = 0.01;
		
		qoptions.numEpisodes = 10;
		qoptions.noiseDecayFactor = [](int numEpisodes) { return 1.0 / (numEpisodes * 0.1); };
		qoptions.learningRate = 0.01;
		qoptions.maxNoise = 1.5;
		qoptions.minNoise = 0.001;
		qoptions.maxStepsWithoutFood = [&](int episode) -> size_t
		{
			return 150u + (double)episode / qoptions.numEpisodes * 300.0;
		};
		
		qoptions.foodReward = [](int episode) { return 1.0; };
		qoptions.dieReward = [&](int episode) { return 0; };
		qoptions.stepReward = [](int episode) { return 0; };
		
//		auto qInitDistrib = std::uniform_real_distribution<>(-1.0, 1.0);
		qoptions.tabInitializer = [&](std::mt19937& merseneTwister)
		{
//			return 0;
			return 0.5;
//			return qInitDistrib(merseneTwister);
		};
		qoptions.milsToSleepBetweenFrames = 25;
		qoptions.lastNGamesToRender = 5;
	}
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

std::string GetGridObserverJsonQTabName(const GridObserver& observer)
{
	return
		std::string() +
		"aux_files/qtabular/" +
		"TrainedGrid" +
			std::to_string(observer.GetWidth()) + 'x' +
			std::to_string(observer.GetHeight()) +
		".json";
}

QTable TryLoadQTab(const GridObserver& observer, std::string rootPath = "")
{
	auto filePath = rootPath + GetGridObserverJsonQTabName(observer);
	
	try
	{
		return AI::QLearning::Utils::LoadQTable(filePath.c_str());
	}
	catch (...)
	{
		std::cerr << "Failed to load QTab: " << filePath << std::endl;
		return QTable();
	}
}

void TrySaveQTab(const GridObserver& observer, const QTable& qtab, std::string rootPath = "")
{
	auto filePath = rootPath + GetGridObserverJsonQTabName(observer);
	
	try
	{
		AI::QLearning::Utils::SaveQTable(qtab, filePath.c_str());
	}
	catch (...)
	{
		std::cerr << "Failed to save QTab: " << filePath << std::endl;
	}
}

void TrySavePlayer(const QTabStudent& student)
{
	const auto filePath = "aux_files/qtabular/TrainedQTabAgent.json";
	std::ofstream outFileStream(filePath);
	
	if (!outFileStream.is_open())
	{
		std::cerr << "Failed to open " << filePath << std::endl;
		return;
	}
	
	try
	{
		outFileStream << std::setw(2) << nlohmann::json(&student);
	}
	catch (...)
	{
		std::cerr << "Failed to save player." << std::endl;
	}
}

IPlayerPtr MultisnakeTabularTrainer::Train(TrainCallbacks callbacks)
{
	m_qoptions.numEpisodes = callbacks.numEpisodes;
	
	auto agents = std::vector<std::shared_ptr<QTabStudent>>();
	
	for (auto i = 0u; i < 1u; i++)
	{
		auto cellInterpreter = std::make_shared<WallFoodEnemy>();
		auto observer = std::make_shared<GridObserver>(cellInterpreter, 5, 5);
		
		auto agent = std::make_shared<QTabStudent>(
			observer,
			[&]() { return m_qoptions.tabInitializer(m_merseneTwister); },
			m_qoptions.actionQualityEps
		);

		agent->SetQTab(TryLoadQTab(*observer));
		agents.push_back(agent);
	}

	auto players = std::vector<std::shared_ptr<IPlayer>>();
	for (auto agent : agents)
	{
		players.push_back(std::static_pointer_cast<IPlayer>(agent));
	}

	auto game = Game(m_gmoptions, players);

	for (const auto agent : agents)
	{
		agent->SetNoise(m_qoptions.maxNoise);
	}
	
	bool startedToRender = false;
	for (auto episode = 0u; episode < m_qoptions.numEpisodes; episode++)
	{
		for (auto& agent : agents)
			agent->PrepareForNewEpisode();
		game.InitGame();

		const auto maxNbOfSteps = m_qoptions.maxNumSteps(episode);
		auto step = 0u;
		for (step = 0u; step < maxNbOfSteps; step++)
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
				const auto actionIndx = agent->PickAction(state);

				assert(actionIndx < IPlayer::possibleMoves.size());
				const auto rawReward = game.MoveSnake(
					snakeId,
					IPlayer::possibleMoves[actionIndx]);
				
				// Compute reward.
				auto reward = 0.0;
				if (rawReward < 0)
					reward += m_qoptions.dieReward(episode);
				else if (rawReward > 0)
					reward += m_qoptions.foodReward(episode);
				else
					reward += m_qoptions.stepReward(episode);
				agent->SetReward(agent->GetReward() + reward);
				agent->m_totalReward += reward;

				State newState;
				double bestNextQ = 0;
				
				const auto isDone = (rawReward < 0);
				if (!isDone)
				{
					const auto gmState = game.GetGameState();
					newState = agent->ObserveState(gmState);
					bestNextQ = agent->GetBestQualityFromState(newState);
				}
				else
				{
					const auto gmState = game.GetGameState();
					assert(!gmState.GetSnake(snakeId).IsAlive());
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
				{
					std::cout << snakeId << " starved to death. ----- " << std::endl;
					game.ForcefullyKillPlayer(snakeId);
				}
				else
					agent->SetStepsWithoutFood(agent->GetStepsWithoutFood() + 1);
			}
			
			if (episode >= m_qoptions.numEpisodes - m_qoptions.lastNGamesToRender)
			{
				if (!startedToRender)
				{
					startedToRender = true;
					std::system("play -q -n synth 1 sin 880");
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				}

				if (m_gmoptions.gameRenderer != nullptr)
					m_gmoptions.gameRenderer->Render(game.GetGameState());

				if (m_qoptions.milsToSleepBetweenFrames != 0)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(
						m_qoptions.milsToSleepBetweenFrames));
				}
			}
			
			game.RestockFood();
		}
		
		if (episode % 100 == 0)
		{
			printf("End of episode: %d\n", episode);
			for (const auto& agent : agents)
			{
				printf("Reward of %4.2f. Noise: %2.2f\n",
					agent->GetReward(),
					agent->GetNoise());
			}
		}
		
		for (auto& agent : agents)
		{
			agent->SetNoise(
				::Utils::Math::Lerp(
					agent->GetNoise(),
					m_qoptions.minNoise,
					m_qoptions.noiseDecayFactor(callbacks.numEpisodes)));
		}
		
		if (episode % 100 == 0 || episode == callbacks.numEpisodes - 1)
		{
			if (callbacks.emitStepEpisode)
				callbacks.emitStepEpisode(static_cast<size_t>(episode + 1));
			
			if (callbacks.emitGraphValues)
			{
				auto graphRewards = std::vector<double>();
				auto rewardSum = 0.0;
				for (const auto& agent : agents)
				{
					const auto reward = agent->GetReward();
					
					rewardSum += reward;
					graphRewards.push_back(reward);
				}
				graphRewards.push_back(rewardSum / agents.size());
				
				// I want the mean to be the first one.
				std::reverse(graphRewards.begin(), graphRewards.end());
				callbacks.emitGraphValues(graphRewards);
			}
		}
		
		if (callbacks.emitDisplayGame)
			callbacks.emitDisplayGame(agents[0], 1000);
	}
	
	auto bestAgent = *std::max_element(agents.begin(), agents.end(),
		[](const auto agent1, const auto agent2)
		{
			return agent1->m_totalReward < agent2->m_totalReward;
		});
	
	TrySavePlayer(*bestAgent);
	return nlohmann::json(bestAgent.get()).get<std::shared_ptr<QTabStudent>>();
}
