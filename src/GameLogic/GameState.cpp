#include "GameState.h"
#include <algorithm>

GameState::GameState(const GameBoard & gameBoard, const std::vector<Snake>& snakes)
	:m_gameBoard(gameBoard), m_snakes(snakes)
{
}

GameState::~GameState()
{
}

const GameBoard & GameState::GetGameBoard() const
{
	return m_gameBoard;
}

const std::vector<Snake>& GameState::GetSnakes() const
{
	return m_snakes;
}

const Snake & GameState::GetSnake(const int snakeNumber) const
{
	return *std::find_if(m_snakes.begin(), m_snakes.end(),
		[snakeNumber](const auto& snake)
		{
			return snake.GetSnakeNumber() == snakeNumber;
		});
}

const std::vector<std::vector<int>> GameState::GetFieldOfView(const Snake& snake, int width, int length) const
{
	std::vector<std::vector<int>> fieldOfView;
	fieldOfView.resize(width);
	for (auto& line : fieldOfView)
	{
		line.resize(length);
	}
	auto left = snake.GetOrientation().Rotate90Left();
	auto right = snake.GetOrientation().Rotate90Right();
	auto origin = snake.GetSnakeHead() + left*(length / 2);

	for(auto x = 0; x<length; ++x)
		for (auto y = 0; y <width; ++y)
		{
			auto pos = origin + right * x + snake.GetOrientation()*y;
			if (m_gameBoard.IsWallOrBeyond(pos))
			{
				fieldOfView[y][x] = -1;
			}
			else 
			{
				fieldOfView[y][x] = m_gameBoard[pos];
			}
		}
	fieldOfView.shrink_to_fit();
	return fieldOfView;
}

