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

TabularTrainer::TabularTrainer() :
	m_merseneTwister(std::random_device()()),
	m_qtable(),
	m_env()
{
}

IPlayer* TabularTrainer::Train()
{
	m_qtable = ::Utils::Matrix::MakeMatrix(
		m_env.GetNumbOfObservations(),
		m_env.actions.size(),
		0 // InitValue
	);
	
	auto randomActionChance = maxRandActionChance;
	auto dieStates = std::map<int, int>();
	
	// Start training.
	for (auto episode = 0; episode < numEpisodes; episode++)
	{
		m_env.Reset();
		auto state = m_env.GetState();
		
		auto episodeReward = 0.0;
		auto prevState = 0;
		for (auto step = 0; step < maxNumSteps; step++)
		{
			const auto trainStepResult = RunStep(state, randomActionChance);
			
			episodeReward += trainStepResult.reward;
			prevState = state;
			state = trainStepResult.newState;
			
			// Render the env on the last episode.
			if (episode == numEpisodes - 1)
				m_env.Render();
			
			// Track die states.
			if (trainStepResult.isDone)
			{
				if (episode > numEpisodes * 0.95)
					dieStates[prevState]++;
				break;
			}
			
			// Update random action chance.
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
	::Utils::Print::PrintTable(m_qtable);

	std::cout << "Die states: " << std::endl;
	for (const auto& pair : dieStates)
		std::cout << pair.first << ") " << pair.second << std::endl;
	
	return new AI::QLearning::TrainedAgent::TrainedTabularAgent(
		m_env.actions,
		m_qtable);
}

/*
** Private methods.
*/

TabularTrainer::TrainStepResult TabularTrainer::RunStep(
	const State currentState,
	const double randomActionChance)
{
	// Get action with a random noise.
	const auto actionIndex = QLearning::Utils::PickAction(
		m_qtable[currentState],
		randomActionChance,
		m_merseneTwister);

	const auto stepResult = m_env.Step(m_env.actions[actionIndex]);
	const auto newState = m_env.GetState();
	const auto reward = ComputeStepReward(stepResult);

	UpdateActionQuality(
		currentState,
		newState,
		actionIndex,
		reward,
		stepResult.isDone);

	auto trainStepResult = TrainStepResult();
	trainStepResult.newState = newState;
	trainStepResult.reward = reward;
	trainStepResult.isDone = stepResult.isDone;
	
	return trainStepResult;
}

double TabularTrainer::ComputeStepReward(
	const GymEnv::StepResult& stepResult) const
{
	auto reward = stepReward;
	
	if (stepResult.reward > 0)
		reward += foodReward;
	else if (stepResult.reward < 0)
		reward += dieReward;
	
	return reward;
}

double TabularTrainer::UpdateActionQuality(
	const int currentState,
	const int newState,
	const int actionIndex,
	const double actionReward,
	const bool isDone)
{
	double bestNextQ;
	
	if (isDone)
		bestNextQ = 0;
	else
	{
		bestNextQ = *std::max_element(
			m_qtable[newState].begin(),
			m_qtable[newState].end());
	}

	const auto currentActionQ = m_qtable[currentState][actionIndex];
	m_qtable[currentState][actionIndex] +=
		learningRate *
		(actionReward + qDiscountFactor * bestNextQ - currentActionQ);
	
	return m_qtable[currentState][actionIndex];
}
