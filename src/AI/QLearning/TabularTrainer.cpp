#include "TabularTrainer.hpp"
#include "TrainedTabularAgent.hpp"
#include "ActionPickingUtils.h"
#include "GymEnv/SingleSnakeRelativeView.hpp"
#include "GameLogic/GameUtils.h"
#include "Utils/MathUtils.h"
#include "Utils/PrintUtils.h"
#include "Utils/MatrixUtils.h"
#include <vector>
#include <random>
#include <cmath>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <map>

using namespace AI::QLearning;

TabularTrainer::TabularTrainer() : m_merseneTwister(std::random_device()())
{
}

IPlayer* TabularTrainer::Train()
{
	auto env = GymEnv::SingleSnakeRelativeView();
	auto qTable = ::Utils::Matrix::MakeMatrix(
		env.GetNumbOfObservations(),
		env.actions.size(),
		0 // InitValue
	);
	
	auto randomActionChance = maxRandActionChance;
	auto dieStates = std::map<int, int>();
	
	// Start training.
	for (auto episode = 0; episode < numEpisodes; episode++)
	{
		env.Reset();
		auto state = env.GetState();
		
		auto episodeReward = 0.0;
		auto prevState = 0;
		for (auto step = 0; step < maxNumSteps; step++)
		{
			// Get action with a random noise.
			const auto actionIndex = QLearning::Utils::PickAction(
				qTable[state],
				randomActionChance,
				m_merseneTwister);
			
			const auto action = env.actions[actionIndex];
			const auto stepResult = env.Step(action);
			const auto newState = env.GetState();
			
			double reward = 0;
			if (stepResult.reward > 0)
				reward += 1;
			else if (stepResult.reward < 0)
				reward -= 1;
			else
				reward -= 0.005;
			
			double bestNextQ;
			if (stepResult.isDone)
			{
				bestNextQ = 0;
			}
			else
			{
				bestNextQ = *std::max_element(
					qTable[newState].begin(),
					qTable[newState].end());
			}

			const auto currentActionQ = qTable[state][actionIndex];
			qTable[state][actionIndex] +=
				learningRate *
				(reward + qDiscountFactor * bestNextQ - currentActionQ);
			
			episodeReward += reward;
			prevState = state;
			state = newState;
			
			if (episode == numEpisodes - 1)
				env.Render();
			
			if (stepResult.isDone)
			{
				if (episode > numEpisodes * 0.95)
					dieStates[prevState]++;
				break;
			}
			
			randomActionChance = ::Utils::Math::Lerp(
				randomActionChance,
				minRandActionChance,
				randActionDecayFactor);
		}
		
		printf(
			"End of episode: %d with a reward of %.2f."
			"Random action chance: %.2f\n",
			episode,
			episodeReward,
			randomActionChance);
	}
	::Utils::Print::PrintTable(qTable);
	
	std::cout << "Die states: " << std::endl;
	for (const auto& pair : dieStates)
	{
		std::cout << pair.first << ") " << pair.second << std::endl;
	}
	
	return new AI::QLearning::TrainedAgent::TrainedTabularAgent(
		env.actions,
		qTable);
}
