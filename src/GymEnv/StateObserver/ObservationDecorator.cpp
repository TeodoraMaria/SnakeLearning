#include "StateObserver/ObservationDecorator.hpp"

using namespace GymEnv::StateObserver;

ObservationDecorator::ObservationDecorator(
	GameLogic::CellInterpreter::ICellInterpreterPtr cellInterpreter,
	std::shared_ptr<IStateObserver> wrapee
) :
	IStateObserver(cellInterpreter),
	m_wrapee(wrapee)
{
}

void ObservationDecorator::Observe(
	std::vector<double>& observationContainer,
	const GameState& gmState,
	int snakeId) const
{
	m_wrapee->Observe(observationContainer, gmState, snakeId);
}
