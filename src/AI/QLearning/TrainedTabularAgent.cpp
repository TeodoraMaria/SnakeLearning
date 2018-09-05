#include "TrainedTabularAgent.hpp"
#include "TabularTrainer.hpp"
#include "ActionPickingUtils.h"
#include "GymEnv/StateExtractor.hpp"
#include <assert.h>

using namespace AI::QLearning::TrainedAgent;

TrainedTabularAgent::TrainedTabularAgent(
	const std::vector<SnakeMove>& actions,
	size_t interpretableBoardParts,
	const AI::QLearning::QTable& qBrains,
	std::shared_ptr<GymEnv::StateObserver::IStateObserver> observer
) :
	m_actions(actions),
	m_interpretableBoardParts(interpretableBoardParts),
	m_qBrains(qBrains),
	m_observer(observer)
{
}

/*
** Getters & setters.
*/

SnakeMove TrainedTabularAgent::GetNextAction(const GameState& gmState) const
{
	std::random_device randomDevice;
	std::mt19937 merseneTwister(randomDevice());
	
	const auto snake = gmState.GetSnake(GetSnakeNumber());
	
	const auto containerLen = m_observer->NbOfObservations();
	auto observationContainer = std::vector<double>(containerLen);
	
	m_observer->Observe(
		observationContainer,
		gmState,
		GetSnakeNumber());
	
	const int state = GymEnv::Utils::StateExtractor::BinaryVectorToNumber(
		observationContainer,
		m_interpretableBoardParts);
	
	const auto actionIndex = QLearning::Utils::PickAction(
		m_qBrains[state],
		0, // Noise
		merseneTwister);
	
	assert(static_cast<size_t>(actionIndex) < m_actions.size());
	return m_actions[actionIndex];
}
