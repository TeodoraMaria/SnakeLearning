#pragma once
#include "Snake.h"
#include "GameBoard.h"

typedef std::vector<std::vector<int>> FieldOfView;

class GameState
{
public:
	GameState(const GameBoard& gameBoard, const std::vector<Snake>& snakes);
	~GameState();

	const GameBoard& GetGameBoard() const;
	const std::vector<Snake>& GetSnakes() const;
	const Snake& GetSnake(int snakeNumber) const;
	FieldOfView GetFieldOfView(const Snake& snake, int width, int length) const;

	bool IsSnakeHead(const Coordinate& coord) const;
	
private:
	
	const GameBoard& m_gameBoard;
	const std::vector<Snake>& m_snakes;
};
