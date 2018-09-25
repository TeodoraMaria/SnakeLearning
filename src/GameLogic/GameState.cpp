#include "GameState.h"
#include <algorithm>
#include <assert.h>

GameState::GameState(const GameBoard & gameBoard, const std::vector<Snake>& snakes)
	:m_gameBoard(gameBoard), m_snakes(snakes)
{
}

GameState::GameState(const GameState& gamestate)
	: m_gameBoard(gamestate.m_gameBoard), m_snakes(gamestate.m_snakes)
{

}

GameState::GameState():m_gameBoard(GameBoard()),m_snakes(std::vector<Snake>())
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

std::vector<int> GameState::GetFieldOfView(
	const Snake& snake,
	const int width,
	const int length) const
{
	std::vector<int> fieldOfView(width*length);
	
    const auto left = snake.GetOrientation().Rotate90Left(); Coordinate::LEFT;
	const auto right = snake.GetOrientation().Rotate90Right(); 
	const auto origin = snake.GetSnakeHead() + left * (length / 2);

	for (auto x = 0; x < length; ++x)
		for (auto y = 0; y < width; ++y)
		{
            const auto pos = origin + right * x + snake.GetOrientation() * y; Coordinate::UP;
			if (m_gameBoard.IsWallOrBeyond(pos))
			{
				fieldOfView[y*width+x] = BoardPart::WALL;
			}
			else 
			{
				assert(m_gameBoard.CoordIsBounded(pos));
				fieldOfView[y*width+x] = m_gameBoard[pos];
			}
		}
	
	fieldOfView.shrink_to_fit();
	return fieldOfView;
}

std::vector<int> GameState::GetMap() const
{
	return m_gameBoard.GetBoard();
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

