#include "GameState.h"

GameState::GameState(const GameBoard & gameBoard, const std::vector<Snake>& snakes)
	:m_gameBoard(gameBoard), m_snakes(snakes)
{
}

GameState::~GameState()
{
}

const GameBoard & GameState::GetGameBoard() const
{
	return m_gameBoard;
}

const std::vector<Snake>& GameState::GetSnakes() const
{
	return m_snakes;
}

