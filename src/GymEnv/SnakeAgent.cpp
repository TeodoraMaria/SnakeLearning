#include "SnakeAgent.hpp"

using namespace GymEnv;

SnakeAgent::SnakeAgent(
	GameLogic::CellInterpreter::ICellInterpreterPtr cellInterpreter,
	std::shared_ptr<ActionPicking::IActionPicker> actionPicker,
	std::shared_ptr<StateObserver::IStateObserver> observer
) :
	m_cellInterpreter(cellInterpreter),
	m_actionPicker(actionPicker),
	m_observer(observer)
{
	m_observationContainer.resize(observer->NbOfObservations());
}

const GameLogic::CellInterpreter::ICellInterpreter& SnakeAgent::GetCellInterpreter() const
{
	return *m_cellInterpreter.get();
}

const StateObserver::IStateObserver& SnakeAgent::GetObserver() const
{
	return *m_observer.get();
}

SnakeMove SnakeAgent::GetNextAction(const GameState& gameState)
{
	m_observer->Observe(m_observationContainer, gameState, GetSnakeNumber());
	return GetNextAction(m_observationContainer);
}

SnakeMove SnakeAgent::GetNextAction(
	const std::vector<double>& observations,
	double noise)
{
	return m_actionPicker->PickMove(m_observationContainer, noise);
}
