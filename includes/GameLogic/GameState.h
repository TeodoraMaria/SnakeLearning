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

private:	
	const GameBoard& m_gameBoard;
	const std::vector<Snake>& m_snakes;
};