#include "StateExtractor.hpp"
#include <array>
#include <cmath>

using namespace GymEnv::Utils;

int StateExtractor::GetRelativeViewStateBase3(
	const GameState& gmState,
	const int snakeId)
{
	const auto& snake = gmState.GetSnake(snakeId);
	const auto& head = snake.GetSnakeHead();
	const auto snakeOrientation = snake.GetOrientation();
	
	const auto left = head + snakeOrientation.Rotate90Left();
	const auto right = head + snakeOrientation.Rotate90Right();
	const auto forward = head + snakeOrientation;
	
	const auto viewGrid = std::vector<Coordinate>({{ left, forward, right }});
	const auto gmBoard = gmState.GetGameBoard();
	const auto getValue = [&](const int i) -> int
	{
		return GetGameCellValue(gmBoard[viewGrid[i]]);
	};
	return ComputeVieGridValue(viewGrid.size(), getValue);
}

int StateExtractor::GetGridViewState(
	const GameBoard& gmBoard,
	const FieldOfView& fieldOfVIew,
	const size_t numCellStates)
{
	const auto height = fieldOfVIew.size();
	const auto width = fieldOfVIew.front().size();
	const auto getValue = [&](const int indx) -> int
	{
		const auto i = indx / width;
		const auto j = indx % width;
		return GetGameCellValue(fieldOfVIew[i][j]);
	};
	return ComputeVieGridValue(height * width, getValue);
}

/*
** Private helpers.
*/

int StateExtractor::ComputeVieGridValue(
	size_t numCells,
	std::function<int (int)> getValue,
	const int base)
{
	int state = 0;
	
	for (auto i = 0u; i < numCells; i++)
	{
		const auto cellValue = getValue(i);
		state += cellValue * std::pow(base, i);
	}
	
	return state;
}

int StateExtractor::GetGameCellValue(const int gameCellValue)
{
	if (gameCellValue == BoardPart::EMPTY)
		return emptySpaceValue;
	if (gameCellValue == BoardPart::WALL || gameCellValue >= startOfSnakeIndexes)
		return wallValue;
	if (gameCellValue == BoardPart::FOOD)
		return foodValue;
	throw;
}
