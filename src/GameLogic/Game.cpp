#include <algorithm>
#include <stdlib.h>
#include <tuple>
#include <ctime>
#include "Game.h"

Game::Game(const GameOptions& gameOptions): m_gameOptions(gameOptions)
{
	m_gameBoard.resize(gameOptions.GetBoardDimension());
	for (size_t index = 0; index < gameOptions.GetBoardDimension(); ++index)
	{
		m_gameBoard[index].resize(gameOptions.GetBoardDimension());
	}
	m_players.resize(gameOptions.GetNumberOfPlayers());
}

Game::~Game()
{
}

void Game::InitGame()
{
	for (auto& line : m_gameBoard)
		for (auto& elem : line)
			elem = 0;

	if (!(m_gameOptions.GetGameBoardType()==GameBoardType::LIMITLESS))
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

void Game::AddSnakeToGame(const size_t& snakeNumber)
{
	Snake snake(snakeNumber);
	snake.InitSnake(m_gameBoard);
	m_snakes.push_back(snake);
}

void Game::InitFood() {
	Coordinate coord;
	for (size_t foodIndex = 0; foodIndex < m_gameOptions.GetFoodPortions(); ++foodIndex)
	{
		do {
			coord.GenerateCoordinate(m_gameBoard.size());
		} while (!coord.CheckCoord(m_gameBoard));
		m_gameBoard[coord.lineCoord][coord.colCoord] = 1;
	}
}


