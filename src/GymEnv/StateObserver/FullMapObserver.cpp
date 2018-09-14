#include "StateObserver/FullMapObserver.hpp"

using namespace GymEnv::StateObserver;

FullMapObserver::FullMapObserver(
	GameLogic::CellInterpreter::ICellInterpreterPtr cellInterpreter,
	size_t maxMapCells
) :
	IStateObserver(cellInterpreter),
	m_maxMapCells(maxMapCells)
{
}

size_t FullMapObserver::NbOfObservations() const
{
	return m_maxMapCells;
}

void FullMapObserver::Observe(
	std::vector<double>& observationContainer,
	const GameState& gmState,
	const int snakeId) const
{
	const auto gmBoard = gmState.GetGameBoard();
	if (gmBoard.GetBoardWidth() * gmBoard.GetBoardLength() > m_maxMapCells)
		throw "There isn't enough space in the observation container for the given board.";
	
	size_t i;
	for (i = 0u; i < gmBoard.GetBoardCells().size(); i++)
	{
	 	observationContainer[i] = GetCellInterpreter()->InterpretCell(
			snakeId,
			static_cast<BoardPart>(gmBoard.GetBoardCells()[i]));
	}
	
	for (; i < observationContainer.size(); i++)
		observationContainer[i] = 0;
}
