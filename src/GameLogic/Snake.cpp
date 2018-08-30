#include "Snake.h"
#include "GameUtils.h"

Snake::Snake(const size_t& snakeNumber):m_snakeNumber(snakeNumber), m_score(0), m_isAlive(true)
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

Coordinate Snake::GetSnakeTail() const
{
	return m_snakeBody.back();
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
		coord.GenerateCoordinate(board.size(), board[0].size());
	} while (!coord.CheckCoord(board));
	m_snakeBody.push_front(coord);
	board[coord.GetX()][coord.GetY()] = m_snakeNumber;

	// generate snake body
	std::vector<Coordinate> directions{ Coordinate::UP, Coordinate::DOWN, Coordinate::LEFT, Coordinate::RIGHT };

	for (size_t snakeDim = 1; snakeDim < 3; snakeDim++)
	{
		for (auto& direction : directions)
		{
			auxCoord = coord + direction;
			if (auxCoord.CheckCoord(board))
			{
				m_snakeBody.push_back(auxCoord);
				coord = auxCoord;
				board[coord.GetX()][coord.GetY()] = m_snakeNumber;
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
	m_score++;
}

size_t Snake::GetSnakeSize() const
{
	return m_snakeBody.size();
}

bool Snake::operator==(const Snake & snake)
{
	return m_snakeNumber==snake.m_snakeNumber;
}

void Snake::Die()
{
	m_snakeBody.clear();
	m_isAlive = false;
}

Coordinate Snake::GetOrientation() const
{
	auto neck = m_snakeBody.begin();
	std::advance(neck, 1);
	return m_snakeBody.front() - *neck;
}

size_t Snake::GetScore() const
{
	return m_score;
}

bool Snake::IsAlive() const
{
	return m_isAlive;
}
