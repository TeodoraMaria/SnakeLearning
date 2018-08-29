#pragma once
#include <list>
#include <vector>
#include "GameUtils.h"

class Snake
{
private:
	size_t m_snakeNumber;
	std::list<Coordinate> m_snakeBody;
public:
	Snake(const size_t& snakeNumber);
	~Snake();
	size_t GetSnakeNumber() const;
	Coordinate GetSnakeHead() const;
	std::list<Coordinate> GetSnakeBody() const;
	bool InitSnake(std::vector<std::vector<int>>& board);
	void Move(const Coordinate& newPosition);
	void Eat(const Coordinate& foodPosition);
	size_t GetSnakeSize() const;
	void Die();
};
