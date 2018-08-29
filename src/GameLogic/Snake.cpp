#include "Snake.h"
#include "GameUtils.h"

Snake::Snake(const size_t& snakeNumber):m_snakeNumber(snakeNumber)
{
}

Snake::~Snake()
{
}

size_t Snake::GetSnakeNumber() const
{
	return m_snakeNumber;
}

Coordinate Snake::GetSnakeHead() const
{
	return m_snakeBody.front();
}

std::list<Coordinate> Snake::GetSnakeBody() const
{
	return m_snakeBody;
}

bool Snake::InitSnake(std::vector<std::vector<int>>& board)
{
	Coordinate coord, auxCoord;
	// generate snake head
	do {
		coord.GenerateCoordinate(board.size());
	} while (!coord.CheckCoord(board));
	m_snakeBody.push_front(coord);
	board[coord.lineCoord][coord.colCoord] = m_snakeNumber;

	// generate snake body
	std::vector<Coordinate::Direction> directions{ Coordinate::UP, Coordinate::DOWN, Coordinate::LEFT, Coordinate::RIGHT };

	for (size_t snakeDim = 1; snakeDim < 3; snakeDim++)
	{
		for (auto& direction : directions)
		{
			auxCoord = coord + direction;
			if (auxCoord.CheckCoord(board))
			{
				m_snakeBody.push_back(auxCoord);
				coord = auxCoord;
				board[coord.lineCoord][coord.colCoord] = m_snakeNumber;
				break;
			}
		}
	}
	if (m_snakeBody.size() == 3)
		return true;
	return false;
}

void Snake::Move(const Coordinate & newPosition)
{
	m_snakeBody.push_front(newPosition);
	m_snakeBody.pop_back();
}

void Snake::Eat(const Coordinate & foodPosition)
{
	m_snakeBody.push_front(foodPosition);
}

size_t Snake::GetSnakeSize() const
{
	return m_snakeBody.size();
}

void Snake::Die()
{
	m_snakeNumber = 0;
	m_snakeBody.clear();
}
