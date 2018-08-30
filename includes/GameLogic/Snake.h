#pragma once
#include <list>
#include <vector>
#include "Coordinate.h"

class Snake
{
public:
	Snake(const size_t& snakeNumber=0);
	~Snake();

	size_t GetSnakeNumber() const;
	Coordinate GetSnakeHead() const;
	Coordinate GetSnakeTail() const;
	std::list<Coordinate> GetSnakeBody() const;
	size_t GetSnakeSize() const;
	Coordinate GetOrientation() const;
	size_t GetScore() const;
	bool IsAlive() const;

	bool operator ==(const Snake& snake);

	//Initializes snake and return true if initialization was succesfull, false otherwise
	bool InitSnake(std::vector<std::vector<int>>& board);
	void Move(const Coordinate& newPosition);
	void Eat(const Coordinate& foodPosition);
	void Die(std::vector<std::vector<int>>& board);

private:
	size_t m_snakeNumber;
	std::list<Coordinate> m_snakeBody;
	size_t m_score;
	bool m_isAlive;
};