#include "TabularAgent.hpp"
#include "TabularActionPicker.cpp"
#include "GymEnv/StateExtractor.hpp"

using namespace AI::QLearning;
using namespace GymEnv::Utils;

TabularAgent::TabularAgent(
	std::shared_ptr<QTabularBrains> brains,
	GameLogic::CellInterpreter::ICellInterpreterPtr cellInterpreter,
	std::shared_ptr<ActionPicking::IActionPicker> actionPicker,
	std::shared_ptr<GymEnv::StateObserver::IStateObserver> observer
) :
	GymEnv::SnakeAgent(cellInterpreter, actionPicker, observer),
	m_brains(brains)
{
}

QTabularBrains& TabularAgent::GetBrains()
{
	return *m_brains.get();
}

State TabularAgent::GetState(const GameState& gmState) const
{
	auto observationContainer =
		std::vector<double>(GetObserver().NbOfObservations());
	
	GetObserver().Observe(observationContainer, gmState, GetSnakeNumber());
	return GetState(observationContainer);
}

State TabularAgent::GetState(const std::vector<double>& rawState) const
{
	return StateExtractor::BinaryVectorToNumber(
		rawState,
		GetCellInterpreter().NbOfInterpretableParts());
}
