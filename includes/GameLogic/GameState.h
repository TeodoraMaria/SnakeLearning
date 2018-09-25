#pragma once
#include "Snake.h"
#include "GameBoard.h"

class GameState
{
public:
	GameState(const GameBoard& gameBoard, const std::vector<Snake>& snakes);
	GameState(const GameState& gamestate);
    GameState();
	~GameState();

	const GameBoard& GetGameBoard() const;
	const std::vector<Snake>& GetSnakes() const;
	const Snake& GetSnake(int snakeNumber) const;
	std::vector<int> GetFieldOfView(const Snake& snake, int width, int length) const;
	std::vector<int> GetMap() const;

	bool IsSnakeHead(const Coordinate& coord) const;
	
private:
	
	const GameBoard& m_gameBoard;
	const std::vector<Snake>& m_snakes;
};
