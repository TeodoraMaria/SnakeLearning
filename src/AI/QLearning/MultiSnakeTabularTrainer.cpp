#include "MultiSnakeTabularTrainer.hpp"
#include "GymEnv/StateExtractor.hpp"
#include "Utils/MathUtils.h"

using namespace AI::QLearning;
using namespace GymEnv::Utils;

MultiSnakeTabularTrainer::MultiSnakeTabularTrainer(
	QOptions& qoptions,
	std::vector<std::shared_ptr<TabularAgent>> agents,
	std::shared_ptr<GymEnv::MultiSnakeEnv> env
) :
	m_qoptions(qoptions),
	m_agents(agents),
	m_env(env)
{
}

IPlayer* MultiSnakeTabularTrainer::Train()
{
	auto trainSession = TrainSession();
	trainSession.noise = m_qoptions.maxRandActionChance;
	
	for (auto episode = 0u; episode < m_qoptions.numEpisodes; episode++)
	{
		trainSession.episode = episode;
		RunEpisode(trainSession);
	}
	return nullptr;
}

/*
** Private helpers.
*/

void MultiSnakeTabularTrainer::RunEpisode(TrainSession& trainSession)
{
	m_env->Reset();
	
	auto rewards = std::unordered_map<int, double>(m_agents.size());
	for (auto agent : m_agents)
		rewards[agent->GetSnakeNumber()] = 0;
	
	m_env->Render();
	auto gmState = m_env->GetState();
	
	auto observations = std::vector<double>();
	const auto maxNbOfSteps = m_qoptions.maxNumSteps(trainSession.episode);
	for (auto step = 0u; step < maxNbOfSteps; step++)
	{
		auto aliveCount = 0;
		
		for (auto agent : m_agents)
		{
			const auto gmState = m_env->GetState();
			if (!gmState.GetSnake(agent->GetSnakeNumber()).IsAlive())
				continue;
			else
				aliveCount++;
			
			observations.resize(agent->GetObserver().NbOfObservations());
			agent->GetObserver().Observe(
				observations,
				gmState,
				agent->GetSnakeNumber());
			
			const auto state = agent->GetState(observations);
			const auto move = agent->GetNextAction(
				observations,
				trainSession.noise);
			const auto stepResult = m_env->Step(agent->GetSnakeNumber(), move);

			const auto newState = stepResult.isDone ?
				0 : agent->GetState(m_env->GetState());

			const auto reward = ComputeStepReward(
				stepResult.reward,
				trainSession.episode);
			
			agent->GetBrains().UpdateActionQuality(
				state,
				newState,
				reward,
				IndexOfMove(move),
				stepResult.isDone);
			
			rewards[agent->GetSnakeNumber()] += reward;
		}
		
		if (aliveCount == 0)
			break;
		
		m_env->Render();
		
		trainSession.noise = ::Utils::Math::Lerp(
			trainSession.noise,
			m_qoptions.minRandActionChance,
			m_qoptions.randActionDecayFactor);
	}
	
	printf(
		"End of episode: %d; Noise = %.4f\n",
		trainSession.episode,
		trainSession.noise);
	std::cout << "Rewards: " << std::endl;
	for (auto agent : m_agents)
		std::cout << rewards[agent->GetSnakeNumber()] << std::endl;
}

double MultiSnakeTabularTrainer::ComputeStepReward(
	const int rawReward,
	const int episode) const
{
	auto reward = m_qoptions.stepReward(episode);
	
	if (rawReward > 0)
		reward += m_qoptions.foodReward(episode);
	else if (rawReward < 0)
		reward += m_qoptions.dieReward(episode);
	
	return reward;
}

