#include "CellInterpreter/WallFoodBody.hpp"

using namespace GameLogic::CellInterpreter;

size_t WallFoodBody::NbOfInterpretableParts() const
{
	return 4;
}

size_t WallFoodBody::InterpretCell(const BoardPart boardPart) const
{
	if (boardPart == BoardPart::EMPTY)
		return 0;
	if (boardPart == BoardPart::WALL)
		return 1;
	if (boardPart == BoardPart::FOOD)
		return 2;
	if (static_cast<int>(boardPart) >= startOfSnakeIndexes)
		return 3;
	throw UnrecognizedBoardPart();
}
