#pragma once
#include "Snake.h"
#include "GameBoard.h"

class GameState
{
public:
	GameState(const GameBoard& gameBoard, const std::vector<Snake>& snakes);
	~GameState();

	const GameBoard& GetGameBoard() const;
	const std::vector<Snake>& GetSnakes() const;
	const Snake& GetSnake(const int snakeNumber) const;
	const std::vector<std::vector<int>> GetFieldOfView(const Snake& snake, const int width = 5, const int length = 5) const;

	bool IsSnakeHead(const Coordinate& coord) const;
	
private:
	
	const GameBoard& m_gameBoard;
	const std::vector<Snake>& m_snakes;
};
