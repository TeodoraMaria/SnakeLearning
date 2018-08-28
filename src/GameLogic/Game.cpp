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

std::vector<std::vector<int>> Game::GetGameBoard() const
{
	return m_gameBoard;
}

std::vector<std::list<std::tuple<int, int, int>>> Game::GetSnakes() const
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
	std::list<std::tuple<int, int, int>> snake;
	std::tuple<int, int> coord, auxCoord;
	// generate snake head
	do {
		auto lineCoord = GenerateCoordinate();
		auto colCoord = GenerateCoordinate();
		coord = std::make_tuple(lineCoord, colCoord);
	} while (!CheckCoord(coord));
	snake.push_back(std::make_tuple(std::get<0>(coord), std::get<1>(coord), snakeNumber));
	m_gameBoard[std::get<0>(coord)][std::get<1>(coord)] = snakeNumber;

	// generate snake body
	for (size_t snakeDim = 1; snakeDim < 3; snakeDim++)
	{
		auxCoord = std::make_tuple(std::get<0>(coord)-1, std::get<1>(coord));
		if (CheckCoord(auxCoord))
		{
			coord = auxCoord;
			snake.push_back(std::make_tuple(std::get<0>(coord), std::get<1>(coord), snakeNumber));
			m_gameBoard[std::get<0>(coord)][std::get<1>(coord)] = snakeNumber;
			continue;
		}
		auxCoord = std::make_tuple(std::get<0>(coord) + 1, std::get<1>(coord));
		if (CheckCoord(auxCoord))
		{
			coord = auxCoord;
			snake.push_back(std::make_tuple(std::get<0>(coord), std::get<1>(coord), snakeNumber));
			m_gameBoard[std::get<0>(coord)][std::get<1>(coord)] = snakeNumber;
			continue;
		}
		auxCoord = std::make_tuple(std::get<0>(coord), std::get<1>(coord)-1);
		if (CheckCoord(auxCoord))
		{
			coord = auxCoord;
			snake.push_back(std::make_tuple(std::get<0>(coord), std::get<1>(coord), snakeNumber));
			m_gameBoard[std::get<0>(coord)][std::get<1>(coord)] = snakeNumber;
			continue;
		}
		auxCoord = std::make_tuple(std::get<0>(coord), std::get<1>(coord)+1);
		if (CheckCoord(auxCoord))
		{
			coord = auxCoord;
			snake.push_back(std::make_tuple(std::get<0>(coord), std::get<1>(coord), snakeNumber));
			m_gameBoard[std::get<0>(coord)][std::get<1>(coord)] = snakeNumber;
			continue;
		}
	}

	// add snake to game
	m_snakes.push_back(snake);
}

void Game::InitFood() {
	std::tuple<int, int> coord;
	for (size_t foodIndex = 0; foodIndex < m_numberOfFoods; ++foodIndex)
	{
		do {
			auto lineCoord = GenerateCoordinate();
			auto colCoord = GenerateCoordinate();
			coord = std::make_tuple(lineCoord, colCoord);
		} while (!CheckCoord(coord));
		m_gameBoard[std::get<0>(coord)][std::get<1>(coord)] = 1;
	}
}

bool Game::CheckCoord(std::tuple<int, int> coord) const
{
	if (std::get<0>(coord) < 0 || std::get<0>(coord) >= m_gameBoard.size())
		return false;
	if (std::get<1>(coord) < 0 || std::get<1>(coord) >= m_gameBoard.size())
		return false;
	return m_gameBoard[std::get<0>(coord)][std::get<1>(coord)] == 0;
}

int Game::GenerateCoordinate() const
{
	return rand() % m_gameBoard.size();
}