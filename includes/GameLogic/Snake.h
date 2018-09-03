#pragma once
#include "Coordinate.h"
#include "GameBoard.h"
#include <list>
#include <vector>

class Snake
{
public:
	Snake(const size_t& snakeNumber=0);
	~Snake();

	size_t GetSnakeNumber() const;
	size_t GetSnakeSize() const;
	size_t GetScore() const;
	
	Coordinate GetSnakeHead() const;
	Coordinate GetSnakeTail() const;
	Coordinate GetOrientation() const;
	std::list<Coordinate> GetSnakeBody() const;
	
	bool IsAlive() const;

	bool operator ==(const Snake& snake);

	//Initializes snake and return true if initialization was succesfull, false otherwise
	bool InitSnake(GameBoard& gameBoard, size_t size = 3);
	void Move(const Coordinate& newPosition);
	void Eat(const Coordinate& foodPosition);
	void Die();

private:
	void GenerateHead(GameBoard& gameBoard);
	void GenerateBody(
		GameBoard& gameBoard,
		const Coordinate& head,
		size_t size);

	size_t m_snakeNumber;
	std::list<Coordinate> m_snakeBody;
	size_t m_score;
	bool m_isAlive;
};
