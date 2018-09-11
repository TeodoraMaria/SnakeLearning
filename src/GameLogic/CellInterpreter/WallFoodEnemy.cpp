#include "CellInterpreter/WallFoodEnemy.hpp"

using namespace GameLogic::CellInterpreter;

void WallFoodEnemy::SetPlayer(std::shared_ptr<IPlayer> player)
{
	m_player = player;
}

size_t WallFoodEnemy::NbOfInterpretableParts() const
{
	return 5;
}

size_t WallFoodEnemy::InterpretCell(const BoardPart boardPart) const
{
	if (boardPart == BoardPart::EMPTY)
		return 0;
	if (boardPart == BoardPart::WALL)
		return 1;
	if (boardPart == BoardPart::FOOD)
		return 2;
	if (m_player->GetSnakeNumber() == static_cast<int>(boardPart))
		return 3;
	if (static_cast<int>(boardPart) >= startOfSnakeIndexes)
		return 4;
	throw UnrecognizedBoardPart();
}
