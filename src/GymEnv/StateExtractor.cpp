#include "StateExtractor.hpp"
#include "Utils/PrintUtils.h"
#include <array>
#include <cmath>
#include <iostream>

using namespace GymEnv::Utils;

static bool ItsAnObstacle(const int gridValue)
{
	return
		(gridValue == BoardPart::WALL) ||
		(gridValue >= startOfSnakeIndexes);
}

void StateExtractor::GetRelativeViewState(
	std::vector<int>& dest,
	const Snake& snake,
	const GameBoard& gmBoard)
{
	const auto& head = snake.GetSnakeHead();
	const auto snakeOrientation = snake.GetOrientation();
	
	const auto left = head + snakeOrientation.Rotate90Left();
	const auto right = head + snakeOrientation.Rotate90Right();
	const auto forward = head + snakeOrientation;
	
	const auto viewGrid = std::array<Coordinate, 3>({{ left, forward, right }});
	for (auto i = 0u; i < viewGrid.size(); i++)
	{
		const auto gridValue = gmBoard[viewGrid[i]];
		const auto wallInputIndx = i * 2 + 0;
		const auto foodInputIndx = i * 2 + 1;
		
		if (gridValue == BoardPart::FOOD)
		{
			dest[foodInputIndx] = 1;
			dest[wallInputIndx] = 0;
		}
		else if (ItsAnObstacle(gridValue))
		{
			dest[wallInputIndx] = 1;
			dest[foodInputIndx] = 0;
		}
		else
		{
			dest[wallInputIndx] = 0;
			dest[foodInputIndx] = 0;
		}
	}
}

/*
** Receives a vector whcih contains values of 0 or 1.
*/

int StateExtractor::BinaryVectorToNumber(const std::vector<int>& binTable)
{
	auto result = 0;
	
	for (auto i = 0u; i < binTable.size() / 2; i++)
	{
		// Duplicated code will be soon removed.
		const auto wallInputIndx = i * 2 + 0;
		const auto foodInputIndx = i * 2 + 1;
		
		auto cellValue = emptySpaceValue;
		if (binTable[wallInputIndx] != 0)
			cellValue = wallValue;
		else if (binTable[foodInputIndx] != 0)
			cellValue = foodValue;
		
		result += cellValue * std::pow(3, i);
	}
	return result;
}

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
	const std::vector<int>& fieldOfVIew,
	const size_t numCellStates)
{
	const auto getValue = [&](const int indx) -> int
	{
		return GetGameCellValue(fieldOfVIew[indx]);
	};
	return ComputeVieGridValue(fieldOfVIew.size(), getValue);
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
