#include "TabularTrainer.hpp"
#include "TrainedTabularAgent.hpp"
#include "ActionPickingUtils.h"

#include "GymEnv/SingleSnakeRelativeView.hpp"
#include "GymEnv/SingleSnakeGridView.hpp"
#include "GameView/TermRenderer.hpp"
#include "Utils/MathUtils.h"
#include "Utils/PrintUtils.h"
#include "Utils/MatrixUtils.h"

#include <assert.h>
#include <iostream>

using namespace AI::QLearning;

TabularTrainer::TabularTrainer(
	QOptions& qoptions,
	GymEnv::SingleSnakeEnvBase* const env) :
	m_merseneTwister(std::random_device()()),
	m_qtable(),
	m_qoptions(qoptions),
	m_env(env)
{
}

IPlayer* TabularTrainer::Train()
{
	m_qtable = ::Utils::Matrix::MakeMatrix(
		m_env->GetNumbOfObservations(),
		m_env->actions.size(),
		[&]() { return m_qoptions.tabInitializer(m_merseneTwister); }
	);
	
	auto trainSession = TrainSession();
	
	trainSession.randomActionChance = m_qoptions.maxRandActionChance;
	trainSession.dieStates = std::map<State, int>();
	
	// Train.
	for (auto episode = 0; episode < m_qoptions.numEpisodes; episode++)
	{
		trainSession.episodeIndex = episode;
		RunEpisode(trainSession);
	}
	
//	::Utils::Print::PrintTable(m_qtable);
//	for (auto i = 0u; i < m_qtable.size(); i++)
//	{
//		const auto& line = m_qtable[i];
//		if (::Utils::Math::Approx(*std::min_element(line.cbegin(), line.cend()), 0))
//			continue;
//		
//		std::cout << i << ") ";
//		for (const auto& val : line)
//		{
//			std::cout << val << " ";
//		}
//		std::cout << std::endl;
//	}
	
	std::cout << "Die states: " << std::endl;
	for (const auto& pair : trainSession.dieStates)
		std::cout << pair.first << ") " << pair.second << std::endl;
	
	return new AI::QLearning::TrainedAgent::TrainedTabularAgent(
		m_env->actions,
		m_qtable);
}

/*
** Private methods.
*/

void TabularTrainer::RunEpisode(TrainSession& trainSession)
{
	m_env->Reset();
	const auto rawState = m_env->GetState();
	auto state = GymEnv::Utils::StateExtractor::BinaryVectorToNumber(
		rawState,
		m_env->GetCellInterpreter()->NbOfInterpretableCells());
	
	auto episodeReward = 0.0;
	auto prevState = 0;
	auto stepsSinceLastFood = 0;
	const auto maxNumSteps = m_qoptions.maxNumSteps(trainSession.episodeIndex);
	for (auto step = 0; step < maxNumSteps; step++)
	{
		const auto trainStepResult = RunStep(
			state,
			trainSession);
		
		episodeReward += trainStepResult.reward;
		prevState = state;
		state = trainStepResult.newState;
		
		if (trainStepResult.reward > 0.01)
			stepsSinceLastFood = 0;
		else
			stepsSinceLastFood++;
		
		const auto maxStepsWIthoutFood =
			m_qoptions.maxStepsWithoutFood(trainSession.episodeIndex);
			
		if (stepsSinceLastFood >= maxStepsWIthoutFood)
			break;
		
		// Render the env on the last episode.
		if (trainSession.episodeIndex >= m_qoptions.numEpisodes - 10)
		{
			m_env->Render();
			std::cout << "PrevState: " << prevState << std::endl;
			std::cout << "CurrentState: " << state << std::endl;
		}
		
		// Track die states.
		if (trainStepResult.isDone)
		{
			if (trainSession.episodeIndex > m_qoptions.numEpisodes * 0.95)
				trainSession.dieStates[prevState]++;
			break;
		}
		
		// Update random action chance.
		trainSession.randomActionChance = ::Utils::Math::Lerp(
			trainSession.randomActionChance,
			m_qoptions.minRandActionChance,
			m_qoptions.randActionDecayFactor);
	}

	printf(
		"End of episode: %d with a reward of %.2f."
		"Random action chance: %.2f\n",
		trainSession.episodeIndex,
		episodeReward,
		trainSession.randomActionChance);
}

TabularTrainer::TrainStepResult TabularTrainer::RunStep(
	const State currentState,
	const TrainSession& trainSession)
{
	assert(currentState < m_qtable.size());

	// Get action with a random noise.
	const auto actionIndex = QLearning::Utils::PickAction(
		m_qtable[currentState],
		trainSession.randomActionChance,
		m_merseneTwister);

	const auto stepResult = m_env->Step(m_env->actions[actionIndex]);
	const auto newRawState = m_env->GetState();
	const auto newState = GymEnv::Utils::StateExtractor::BinaryVectorToNumber(
		newRawState,
		m_env->GetCellInterpreter()->NbOfInterpretableCells());

	assert(newState < m_qtable.size());
	
	const auto reward = ComputeStepReward(
		stepResult,
		trainSession.episodeIndex);

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
	const GymEnv::StepResult& stepResult,
	const int episode) const
{
	auto reward = m_qoptions.stepReward(episode);
	
	if (stepResult.reward > 0)
		reward += m_qoptions.foodReward(episode);
	else if (stepResult.reward < 0)
		reward += m_qoptions.dieReward(episode);
	
	return reward;
}

double TabularTrainer::UpdateActionQuality(
	const int currentState,
	const int newState,
	const int actionIndex,
	const double reward,
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
		m_qoptions.learningRate *
		(reward + m_qoptions.qDiscountFactor * bestNextQ - currentActionQ);
	
	return m_qtable[currentState][actionIndex];
}
