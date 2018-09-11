#include "CellInterpreter/Basic3CellInterpreter.hpp"

using namespace GameLogic::CellInterpreter;

size_t Basic3CellInterpreter::NbOfInterpretableParts() const
{
	return 3;
}

size_t Basic3CellInterpreter::InterpretCell(int playerId, const BoardPart boardPart) const
{
	(void)playerId;
	
	if (boardPart == BoardPart::EMPTY)
		return 0;
	if (GameBoard::IsObstacle(boardPart))
		return 1;
	if (boardPart == BoardPart::FOOD)
		return 2;
	throw UnrecognizedBoardPart();
}
