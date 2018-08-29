#include <algorithm>
#include <stdlib.h>
#include <tuple>
#include <ctime>
#include "Game.h"

Game::Game(bool limitless, size_t boardDimension, size_t numberOfPlayers, size_t foodNo)
	:m_isLimitless(limitless), m_numberOfFoods(foodNo)
{
	m_gameBoard.resize(boardDimension);
	for (size_t index = 0; index < boardDimension; ++index)
	{
		m_gameBoard[index].resize(boardDimension);
	}
	m_players.resize(numberOfPlayers);
}

Game::~Game()
{
}

void Game::InitGame()
{
	for (auto& line : m_gameBoard)
		for (auto& elem : line)
			elem = 0;

	if (!m_isLimitless)
	{
		AddLimitsToBoard();
	}
	InitSnakes();
	InitFood();
}

Game::GameBoard Game::GetGameBoard() const
{
	return m_gameBoard;
}

std::vector<Snake> Game::GetSnakes() const
{
	return m_snakes;
}

void Game::AddLimitsToBoard()
{
	size_t dimension = m_gameBoard.size() - 1;
	for (size_t index = 0; index <= dimension; ++index)
	{
		m_gameBoard[index][0] = -1;
		m_gameBoard[index][dimension] = -1;
		m_gameBoard[0][index] = -1;
		m_gameBoard[dimension][index] = -1;
	}
}

void Game::InitSnakes()
{
	size_t snakeIndex = 10;
	for (auto& player : m_players)
	{
		++snakeIndex;
		AddSnakeToGame(snakeIndex);
		player.SetSnakeNumber(snakeIndex);
	}
}

void Game::AddSnakeToGame(size_t snakeNumber)
{
	Snake snake(snakeNumber);
	snake.InitSnake(m_gameBoard);
	m_snakes.push_back(snake);
}

void Game::InitFood() {
	Coordinate coord;
	for (size_t foodIndex = 0; foodIndex < m_numberOfFoods; ++foodIndex)
	{
		do {
			coord.GenerateCoordinate(m_gameBoard.size());
		} while (!coord.CheckCoord(m_gameBoard));
		m_gameBoard[coord.lineCoord][coord.colCoord] = 1;
	}
}


