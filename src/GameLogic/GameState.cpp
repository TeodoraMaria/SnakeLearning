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

const FieldOfView GameState::GetFieldOfView(
	const Snake& snake,
	const int width,
	const int length) const
{
	FieldOfView fieldOfView(width);
	
	for (auto& line : fieldOfView)
		line.resize(length);
	
	const auto left = snake.GetOrientation().Rotate90Left();
	const auto right = snake.GetOrientation().Rotate90Right();
	const auto origin = snake.GetSnakeHead() + left * (length / 2);

	for (auto x = 0; x < length; ++x)
		for (auto y = 0; y < width; ++y)
		{
			const auto pos = origin + right * x + snake.GetOrientation() * y;
			if (m_gameBoard.IsWallOrBeyond(pos))
			{
				fieldOfView[y][x] = BoardPart::WALL;
			}
			else 
			{
				fieldOfView[y][x] = m_gameBoard[pos];
			}
		}
	
	fieldOfView.shrink_to_fit();
	return fieldOfView;
}

bool GameState::IsSnakeHead(const Coordinate& coord) const
{
	if (m_gameBoard[coord] <= startOfSnakeIndexes)
		return false;

	auto snakeNumber = m_gameBoard[coord];
	auto snake = *std::find_if(
		m_snakes.begin(),
		m_snakes.end(),
		[snakeNumber](const auto& snake)
	{
		return snake.GetSnakeNumber() == static_cast<size_t>(snakeNumber);
	});

	auto head = snake.GetSnakeHead();

	return (head == coord);
}

