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
#include <chrono>
#include <thread>
#include <algorithm>

using namespace AI::QLearning;

TabularTrainer::TabularTrainer(
	QOptions& qoptions,
	GymEnv::SingleSnakeEnvBase* const env
) :
	m_merseneTwister(std::random_device()()),
	m_qtable(),
	m_qoptions(qoptions),
	m_env(env)
{
}

const QTable& TabularTrainer::GetQTable() const
{
	return m_qtable;
}

void TabularTrainer::SetQTable(const QTable& qtable)
{
	m_qtable = qtable;
}

static void PrintDieStates(
	const std::unordered_map<unsigned long long, int>& dieStates)
{
	std::cout << "Die states: " << std::endl;
	for (const auto& pair : dieStates)
		std::cout << pair.first << ") " << pair.second << std::endl;
}

IPlayer* TabularTrainer::Train()
{
	auto trainSession = TrainSession();
	trainSession.randomActionChance = m_qoptions.maxRandActionChance;
	trainSession.dieStates = std::unordered_map<State, int>();
	
	// Train.
	for (auto episode = 0; episode < m_qoptions.numEpisodes; episode++)
	{
		trainSession.episodeIndex = episode;
		RunEpisode(trainSession);
	}
	
	if (m_qoptions.printDieStates)
		PrintDieStates(trainSession.dieStates);
	
	return new AI::QLearning::TrainedAgent::TrainedTabularAgent(
		m_env->actions,
		m_env->GetCellInterpreter()->NbOfInterpretableParts(),
		m_qtable,
		std::shared_ptr<GymEnv::StateObserver::IStateObserver>(
			m_env->GetObserver()->Clone()));
}

/*
** Private methods.
*/

bool TabularTrainer::IsRenderPhase(const int episode) const
{
	return (episode >= m_qoptions.numEpisodes - m_qoptions.lastNGamesToRender);
}

void TabularTrainer::RunEpisode(TrainSession& trainSession)
{
	m_env->Reset();
	const auto rawState = m_env->GetState();
	State state = GymEnv::Utils::StateExtractor::BinaryVectorToNumber(
		rawState,
		m_env->GetCellInterpreter()->NbOfInterpretableParts());
	
	auto episodeReward = 0.0;
	State prevState = 0;
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
		
		if (IsRenderPhase(trainSession.episodeIndex))
		{
			m_env->Render();
			std::cout << "PrevState: " << prevState << std::endl;
			std::cout << "CurrentState: " << state << std::endl;
			
			if (m_qoptions.milsToSleepBetweenFrames != 0)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(
					m_qoptions.milsToSleepBetweenFrames));
			}
		}
		
		if (trainStepResult.isDone)
		{
			if (m_qoptions.printDieStates)
			{
				const auto trackDieStateIndxStart =
					m_qoptions.numEpisodes * (1 - m_qoptions.dieStatesGamePart);
				
				if (trainSession.episodeIndex > trackDieStateIndxStart)
					trainSession.dieStates[prevState]++;
			}
			
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
	// Get action with a random noise.
	TryInitQField(currentState);
	const auto actionIndex = QLearning::Utils::PickAction(
		m_qtable[currentState],
		trainSession.randomActionChance,
		m_merseneTwister,
		m_qoptions.actionQualityEps);
	
	std::vector<double> newRawState;
	State newState = 0;

	const auto stepResult = m_env->Step(m_env->actions[actionIndex]);
	const auto reward = ComputeStepReward(
		stepResult,
		trainSession.episodeIndex);
	
	if (!stepResult.isDone)
	{
		newRawState = m_env->GetState();
		newState = GymEnv::Utils::StateExtractor::BinaryVectorToNumber(
			newRawState,
			m_env->GetCellInterpreter()->NbOfInterpretableParts());

		UpdateActionQuality(
			currentState,
			newState,
			actionIndex,
			reward,
			stepResult.isDone);
	}
	
	auto trainStepResult = TrainStepResult();
	trainStepResult.newState = newState;
	trainStepResult.reward = reward;
	trainStepResult.isDone = stepResult.isDone;

	if (IsRenderPhase(trainSession.episodeIndex) && !stepResult.isDone)
	{
		std::cout << "Computed state: " << newState << std::endl;
		::Utils::Print::PrintMatrix(
			newRawState,
			m_env->GetCellInterpreter()->NbOfInterpretableParts() - 1);
	}

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
	const State currentState,
	const State newState,
	const int actionIndex,
	const double reward,
	const bool isDone)
{
	double bestNextQ;
	
	if (isDone)
		bestNextQ = 0;
	else
	{
		TryInitQField(newState);
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

void TabularTrainer::TryInitQField(const State key)
{
	if (m_qtable.find(key) == m_qtable.end())
	{
		m_qtable[key] = ::Utils::Matrix::MakeVector(
			m_env->actions.size(),
			[&]() { return m_qoptions.tabInitializer(m_merseneTwister); });
	}
}
