#include "TabularTrainer.hpp"
#include "GymEnv/SingleSnakeRelativeView.hpp"
#include "GameLogic/GameUtils.h"
#include <vector>
#include <random>
#include <cmath>
#include <assert.h>

using namespace AI::QLearning;

static double lerp(const double a, const double b, const double scalar)
{
	return a + (b - a) * scalar;
}

void TabularTrainer::Train()
{
	auto env = GymEnv::SingleSnakeRelativeView();
	
	auto qTable = std::vector<std::vector<double>>();
	for (auto i = 0u; i < env.GetNumbOfObservations(); i++)
		qTable.push_back(std::vector<double>(env.actions.size()));
	assert(qTable.size() == env.GetNumbOfObservations());
	
	// Set hyperparameters.
	const auto learningRate = 0.1;
	const auto qDiscountFactor = 0.95;
	const auto numEpisodes = 300;
	auto maxNumSteps = 500;
	
	const auto maxRandActionChance = 0.9;
	const auto minRandActionChance = 0.05;
	const auto randActionDecayFactor = 1.0 / 5000;
	auto randomActionChance = maxRandActionChance;
	
	std::random_device randomDevice;
	std::mt19937 merseneTwister(randomDevice());
	std::uniform_real_distribution<double> chanceDistrib(0, 1.0);

	// Start training.
	auto totalNbOfSteps = 0;
	for (auto episode = 0; episode < numEpisodes; episode++)
	{
		env.Reset();
		auto state = env.GetState();
		auto episodeReward = 0.0;
		
		for (auto step = 0; step < maxNumSteps; step++)
		{
			// Get action with a random noise.
			auto actionIndex = 0u;
			if (chanceDistrib(merseneTwister) < randomActionChance)
			{
				actionIndex = rand() % env.actions.size();
			}
			else
			{
				const auto qActions = qTable[state];
				const auto maxAction = std::max_element(
					qActions.begin(),
					qActions.end());
				
				actionIndex = std::distance(
					qActions.begin(),
					maxAction);
			}
			
			const auto action = env.actions[actionIndex];
			const auto stepResult = env.Step(action);
			const auto newState = env.GetState();
			
			const auto currentActionQ = qTable[state][actionIndex];
			const auto bestNextQ = *std::max(
				qTable[newState].begin(),
				qTable[newState].end());
			
			const auto reward = stepResult.reward;
			qTable[state][actionIndex] +=
				learningRate *
				(reward + qDiscountFactor * bestNextQ - currentActionQ);
			
			episodeReward += stepResult.reward;
			state = newState;
			
			if (stepResult.isDone)
				break;

			randomActionChance = lerp(
				minRandActionChance,
				randomActionChance,
				randActionDecayFactor);
			totalNbOfSteps++;
		}
		
		printf(
			"End of episode: %d with a reward of %.2f."
			"Random action chance: %.2f\n",
			episode,
			episodeReward,
			randomActionChance);
	}
}
