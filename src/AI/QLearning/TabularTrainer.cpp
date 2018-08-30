#include "TabularTrainer.hpp"
#include "GymEnv/SingleSnakeRelativeView.hpp"
#include "GameLogic/GameUtils.h"
#include <vector>
#include <random>
#include <cmath>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <map>

using namespace AI::QLearning;

static double lerp(const double a, const double b, const double scalar)
{
	return a + (b - a) * scalar;
}

void printTable(std::vector<std::vector<double>>& table)
{
	for (auto i = 0u; i < table.size(); i++)
	{
		std::cout << std::setw(5) << i << ") ";
		for (auto j = 0u; j < table[i].size(); j++)
		{
			std::cout
				<< std::setw(18) << std::fixed
				<< std::setprecision(5) << table[i][j];
		}
		std::cout << std::endl;
	}
}

bool approx(const double a, const double b)
{
	return std::abs(a - b) < std::numeric_limits<double>::epsilon();
}

void TabularTrainer::Train()
{
	auto env = GymEnv::SingleSnakeRelativeView();
	
	auto qTable = std::vector<std::vector<double>>();
	for (auto i = 0u; i < env.GetNumbOfObservations(); i++)
		qTable.push_back(std::vector<double>(env.actions.size()));
	assert(qTable.size() == env.GetNumbOfObservations());
	
	for (auto& line : qTable)
	{
		for (auto& value : line)
		{
			value = 0;
		}
	}
	
	// Set hyperparameters.
	const auto learningRate = 0.5;
	const auto qDiscountFactor = 0.99;
	const auto numEpisodes = 1000;
	auto maxNumSteps = 30000;
	
	const auto maxRandActionChance = 0.9;
	const auto minRandActionChance = 0.00;
	const auto randActionDecayFactor = 1.0 / 1000;
	auto randomActionChance = maxRandActionChance;
	
	std::random_device randomDevice;
	std::mt19937 merseneTwister(randomDevice());
	std::uniform_real_distribution<double> chanceDistrib(0, 1.0);
	std::uniform_int_distribution<int> actionDistrib(0, env.actions.size() - 1);

	auto dieStates = std::map<int, int>();
	
	// Start training.
	auto totalNbOfSteps = 0;
	for (auto episode = 0; episode < numEpisodes; episode++)
	{
		env.Reset();
		auto state = env.GetState();
//		env.Render();
		
		auto episodeReward = 0.0;
		auto prevState = 0;
		for (auto step = 0; step < maxNumSteps; step++)
		{
			// Get action with a random noise.
			auto actionIndex = 0u;
			if (chanceDistrib(merseneTwister) < randomActionChance)
			{
				actionIndex = actionDistrib(merseneTwister);
			}
			else
			{
				const auto qActions = qTable[state];
				const auto maxActionQ = *std::max_element(
					qActions.begin(),
					qActions.end());
				
				const auto maxElementsCount = std::count_if(
					qActions.begin(),
					qActions.end(),
					[&](const auto& actionQ)
					{
						return approx(actionQ, maxActionQ);
					});
				
				auto maxActionDistrib = std::uniform_int_distribution<int>(0, maxElementsCount - 1);
				auto maxActionIndex = maxActionDistrib(merseneTwister);
				
				for (auto i = 0u; i < qActions.size(); i++)
				{
					if (approx(qActions[i], maxActionQ))
					{
						if (maxActionIndex == 0)
						{
							actionIndex = i;
							break;
						}
						
						maxActionIndex--;
					}
				}
			}
			
			const auto action = env.actions[actionIndex];
			const auto stepResult = env.Step(action);
			const auto newState = env.GetState();
			
			const auto currentActionQ = qTable[state][actionIndex];
			const auto bestNextQ = *std::max(
				qTable[newState].begin(),
				qTable[newState].end());
			
			double reward = 0;
			if (stepResult.reward > 0)
				reward += 1;
			else if (stepResult.reward < 0)
				reward -= 1;
			else
				reward -= 0.1;
			
			
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
//			if (episodeReward < 0.1 && episodeReward > -0.1 && step > 30)
//			{
//				std::cout << "Forced kill!" << std::endl;
//				break;
//			}
			
			randomActionChance = lerp(
				randomActionChance,
				minRandActionChance,
				randActionDecayFactor);
			totalNbOfSteps++;
		}
		
		printf(
			"End of episode: %d with a reward of %.2f."
			"Random action chance: %.2f\n",
			episode,
			episodeReward,
			randomActionChance);
		
//		if (episode % 100 == 0)
//		{
//			printTable(qTable);
//		}
	}
	printTable(qTable);
	
	std::cout << "Die states: " << std::endl;
	for (const auto& pair : dieStates)
	{
		std::cout << pair.first << ") " << pair.second << std::endl;
	}
}
