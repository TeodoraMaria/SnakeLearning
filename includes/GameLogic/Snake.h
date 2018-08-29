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
	size_t GetSnakeSize() const;

	//Initializes snake and return true if initialization was succesfull, false otherwise
	bool InitSnake(std::vector<std::vector<int>>& board);
	void Move(const Coordinate& newPosition);
	void Eat(const Coordinate& foodPosition);
	void Die();
};