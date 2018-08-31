#include "TrainedTabularAgent.hpp"
#include "TabularTrainer.hpp"

using namespace AI::QLearning::TrainedAgent;

TrainedTabularAgent::TrainedTabularAgent(
	const std::vector<SnakeMove>& actions,
	const QTable& qBrains) :
	m_stateExtractor(),
	m_actions(actions),
	m_qBrains(qBrains)
{
}

/*
** Getters & setters.
*/

SnakeMove TrainedTabularAgent::GetNextAction(const GameState& gameState) const
{
	std::random_device randomDevice;
	std::mt19937 merseneTwister(randomDevice());
	
	const auto state = m_stateExtractor.GetRelativeViewStateBase3(
		gameState,
		GetSnakeNumber());
	
	const auto actionIndex = TabularTrainer::GetAction(
		m_qBrains[state],
		0,
		merseneTwister);
	
	assert(actionIndex < m_actions.size());
	return m_actions[actionIndex];
}
