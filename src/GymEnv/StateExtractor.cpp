#include "StateExtractor.hpp"
#include <array>
#include <cmath>

using namespace GymEnv::Utils;

int StateExtractor::GetRelativeViewStateBase3(
	const GameState& gmState,
	const int snakeId) const
{
	const auto& snake = gmState.GetSnake(snakeId);
	const auto& head = snake.GetSnakeHead();
	const auto snakeOrientation = snake.GetOrientation();
	
	const auto left = head + snakeOrientation.Rotate90Left();
	const auto right = head + snakeOrientation.Rotate90Right();
	const auto forward = head + snakeOrientation;
	
	const auto viewGrid = std::vector<Coordinate>({{ left, forward, right }});
	return ComputeVieGridValue(gmState.GetGameBoard(), viewGrid);
}

/*
** Private helpers.
*/

int StateExtractor::ComputeVieGridValue(
	const GameBoard& gmBoard,
	const std::vector<Coordinate>& viewGrid,
	const int base) const
{
	int state = 0;
	
	for (auto i = 0u; i < viewGrid.size(); i++)
	{
		if (gmBoard[viewGrid[i]] == 0)
			continue;
		
		auto cellValue = StateExtractor::emptySpaceValue;
		
		// Non food.
		if (gmBoard[viewGrid[i]] != BoardPart::FOOD)
			cellValue = StateExtractor::wallValue;
		else
			cellValue = StateExtractor::foodValue;
		
		state += cellValue * std::pow(base, i);
	}
	
	return state;
}
