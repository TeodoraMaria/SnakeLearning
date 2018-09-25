#include "Snake.h"
#include "GameUtils.h"
#include <assert.h>

static const auto directions =
{
	Coordinate::UP,
	Coordinate::DOWN,
	Coordinate::LEFT,
	Coordinate::RIGHT
};

Snake::Snake(const size_t& snakeNumber) :
	m_snakeNumber(snakeNumber),
	m_score(0),
	m_isAlive(true)
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

Coordinate Snake::GetSnakeNeck() const
{
	auto neck = m_snakeBody.begin();
	std::advance(neck, 1);
	return *neck;
}

Coordinate Snake::GetSnakeTail() const
{
	return m_snakeBody.back();
}

std::list<Coordinate> Snake::GetSnakeBody() const
{
	return m_snakeBody;
}

bool Snake::InitSnake(GameBoard& gameBoard, const size_t size)
{
	assert((size > 1) && "Invalid snake size. Minimum: 2");
	
	GenerateHead(gameBoard);
	GenerateBody(gameBoard, GetSnakeHead(), size);
	
	if (m_snakeBody.size() == size)
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

void Snake::GenerateHead(GameBoard & gameBoard)
{
	Coordinate coord = gameBoard.GenerateCoordinate();
	m_snakeBody.push_front(coord);
	gameBoard[coord] = m_snakeNumber;
}

void Snake::GenerateBody(
	GameBoard & gameBoard,
	const Coordinate & head,
	size_t size)
{
	Coordinate coord, auxCoord;
	coord = head;

	for (size_t snakeDim = 1; snakeDim < size; snakeDim++)
	{
		for (auto& direction : directions)
		{
			auxCoord = coord + direction;
			if (gameBoard.CoordIsEmpty(auxCoord))
			{
				m_snakeBody.push_back(auxCoord);
				coord = auxCoord;
				gameBoard[coord] = m_snakeNumber;
				break;
			}
		}
	}
}

Coordinate Snake::GetOrientation() const
{
	assert(m_snakeBody.size() > 1);
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
