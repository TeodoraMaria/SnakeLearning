#include "TabularTrainer.hpp"
#include "TrainedTabularAgent.hpp"
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

static bool approx(
	const double a,
	const double b,
	const double tolerance = 0.00001)
{
	return std::abs(a - b) < tolerance;
}

static size_t countElementsWithValue(
	const std::vector<double>& tab,
	const double targetValue)
{
	return std::count_if(
		tab.begin(),
		tab.end(),
		[&](const auto& value)
		{
			return approx(value, targetValue);
		});
}

int TabularTrainer::GetAction(
	const std::vector<double>& qActions,
	const double noise,
	std::mt19937& merseneTwister)
{
	auto chanceDistrib = std::uniform_real_distribution<double>(0, 1.0);
	
	// Random action.
	if (chanceDistrib(merseneTwister) < noise)
	{
		auto actionDistrib = std::uniform_int_distribution<int>(
			0,
			qActions.size() - 1);
		
		return actionDistrib(merseneTwister);
	}
	
	// If multiple actions have the same max quality, choose a random one from
	// those.
	const auto maxActionQ = *std::max_element(qActions.begin(), qActions.end());
	const auto maxElementsCount = countElementsWithValue(qActions, maxActionQ);

	// - Get the index from the max elements.
	auto maxActionIndex = 0;
//	if (maxElementsCount == 1)
//		maxActionIndex = 0;
//	else
//	{
//		auto maxActionDistrib = std::uniform_int_distribution<int>(
//			0, maxElementsCount - 1);
//		maxActionIndex = maxActionDistrib(merseneTwister);
//	}

	// - Get the maxActionIndex'th max element.
	for (auto i = 0u; i < qActions.size(); i++)
	{
		if (approx(qActions[i], maxActionQ))
		{
			if (maxActionIndex == 0)
				return i;
			
			maxActionIndex--;
		}
	}
	throw;
}

static double lerp(const double a, const double b, const double scalar)
{
	return a + (b - a) * scalar;
}

static void printTable(std::vector<std::vector<double>>& table)
{
	for (auto i = 0u; i < table.size(); i++)
	{
		std::cout << std::setw(5) << i << ") ";
		for (auto j = 0u; j < table[i].size(); j++)
		{
			std::cout
				<< std::setw(22) << std::fixed
				<< std::setprecision(16) << table[i][j];
		}
		std::cout << std::endl;
	}
}

IPlayer* TabularTrainer::Train()
{
	auto env = GymEnv::SingleSnakeRelativeView();
	
	auto qTable = std::vector<std::vector<double>>(env.GetNumbOfObservations());
	for (auto i = 0u; i < qTable.size(); i++)
		qTable[i] = std::vector<double>(env.actions.size());
	assert(qTable.size() == env.GetNumbOfObservations());
	
	for (auto& line : qTable)
	{
		for (auto& value : line)
		{
			value = 0;
		}
	}
	
	// Set hyperparameters.
	const auto learningRate = 0.1;
	const auto qDiscountFactor = 0.99;
	const auto numEpisodes = 1000;
	auto maxNumSteps = 10000;
	
	const auto maxRandActionChance = 0.9;
	const auto minRandActionChance = 0.00;
	const auto randActionDecayFactor = 1.0 / 8000;
	auto randomActionChance = maxRandActionChance;
	
	std::random_device randomDevice;
	std::mt19937 merseneTwister(randomDevice());

	auto dieStates = std::map<int, int>();
	
	// Start training.
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
			const auto actionIndex = GetAction(
				qTable[state],
				randomActionChance,
				merseneTwister);
			
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
//			if (episodeReward < 0.1 && episodeReward > -0.1 && step > 30)
//			{
//				std::cout << "Forced kill!" << std::endl;
//				break;
//			}
			
			randomActionChance = lerp(
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
	
	return new AI::QLearning::TrainedAgent::TrainedTabularAgent(
		env.actions,
		qTable);
}
