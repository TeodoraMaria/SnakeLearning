#include "CellInterpreter/WallFoodEnemy.hpp"

using namespace GameLogic::CellInterpreter;

size_t WallFoodEnemy::NbOfInterpretableParts() const
{
	return 5;
}

size_t WallFoodEnemy::InterpretCell(int playerId, const BoardPart boardPart) const
{
	if (boardPart == BoardPart::EMPTY)
		return 0;
	if (boardPart == BoardPart::WALL)
		return 1;
	if (boardPart == BoardPart::FOOD)
		return 2;
	if (playerId == static_cast<int>(boardPart))
		return 3;
	if (static_cast<int>(boardPart) >= startOfSnakeIndexes)
		return 4;
	throw UnrecognizedBoardPart();
}
