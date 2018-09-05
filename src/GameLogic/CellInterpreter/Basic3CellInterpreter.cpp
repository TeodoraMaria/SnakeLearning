#include "CellInterpreter/Basic3CellInterpreter.hpp"

using namespace GameLogic::CellInterpreter;

size_t Basic3CellInterpreter::NbOfInterpretableCells() const
{
	return 3;
}

size_t Basic3CellInterpreter::InterpretCell(const BoardPart boardPart) const
{
	if (boardPart == BoardPart::EMPTY)
		return 0;
	if (GameBoard::IsObstacle(boardPart))
		return 1;
	if (boardPart == BoardPart::FOOD)
		return 2;
	throw UnrecognizedBoardPart();
}
