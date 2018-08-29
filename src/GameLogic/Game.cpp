#include <algorithm>
#include <stdlib.h>
#include <tuple>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include "Game.h"

Game::Game(const GameOptions& gameOptions, const std::vector<HumanPlayer>& players)
	: m_gameOptions(gameOptions), m_players(players)
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

bool Game::IsFood(const Coordinate & location)
{
	return m_gameBoard[location.GetX()][location.GetY()]==1;
}

void Game::CheckIfGameOver()
{
	m_isGameOver = (m_snakes.size() == 0);
}

void Game::PrintBoard()
{
	HANDLE  hConsole;
	int k;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (size_t i = 0; i<m_gameBoard.size(); i++)
	{
		for (size_t j = 0; j<m_gameBoard.size(); j++)
		{
			k = IsSnakeHead(i, j) ? 12 : 15;
			SetConsoleTextAttribute(hConsole, k);
			std::cout << std::setw(4) << m_gameBoard[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void Game::MoveSnake(const size_t & snakeNumber, const int& move)
{
	auto snakeToMoveIterator = std::find_if(m_snakes.begin(), m_snakes.end(), 
		[snakeNumber](const auto& snake) 
		{
		return snake.GetSnakeNumber() == snakeNumber; 
		});

	auto& snakeToMove = *snakeToMoveIterator;
	Coordinate snakeOrientation = snakeToMove.GetOrientation();
	if (move == -1)
	{
		snakeOrientation = snakeOrientation.Rotate90Left();
	}
	else if (move == 1)
	{
		snakeOrientation = snakeOrientation.Rotate90Right();
	}

	Coordinate newSnakeHeadPosition = snakeToMove.GetSnakeHead()+snakeOrientation;

	if (IsFood(newSnakeHeadPosition))
	{
		snakeToMove.Eat(newSnakeHeadPosition);
		m_gameBoard[newSnakeHeadPosition.GetX()][newSnakeHeadPosition.GetY()] = snakeNumber;
		InitFood(1);
	}
	else if (newSnakeHeadPosition.CheckCoord(m_gameBoard))
	{
		Coordinate freedPosition = snakeToMove.GetSnakeTail();
		m_gameBoard[freedPosition.GetX()][freedPosition.GetY()] = 0;
		snakeToMove.Move(newSnakeHeadPosition);
		m_gameBoard[newSnakeHeadPosition.GetX()][newSnakeHeadPosition.GetY()] = snakeNumber;
	}
	else {
		m_snakes.erase(snakeToMoveIterator);
		snakeToMove.Die(m_gameBoard);
	}
	
}

void Game::RunRound()
{
	PrintBoard();
	std::random_shuffle(m_players.begin(), m_players.end());
	for (const auto& player : m_players)
	{
		auto chosenMove = player.GetNextAction();
		size_t snakeNumber = player.GetSnakeNumber();
		MoveSnake(snakeNumber, chosenMove);
	}
}

void Game::Play()
{
	while (!m_isGameOver)
	{
		RunRound();
		CheckIfGameOver();
	}
}

void Game::InitFood(const size_t& foodPortions) {
	Coordinate coord;
	size_t actualFoodPortions = foodPortions == 0 ? m_gameOptions.GetFoodPortions() : foodPortions;

	for (size_t foodIndex = 0; foodIndex <actualFoodPortions; ++foodIndex)
	{
		do {
			coord.GenerateCoordinate(m_gameBoard.size());
		} while (!coord.CheckCoord(m_gameBoard));
		m_gameBoard[coord.GetX()][coord.GetY()] = 1;
	}
}

bool Game::IsSnakeHead(const int& i, const int& j) const
{
	if (m_gameBoard[i][j] <= 10)
		return false;
	size_t snakeNumber = m_gameBoard[i][j];
	auto snake = *std::find_if(m_snakes.begin(), m_snakes.end(),
		[snakeNumber](const auto& snake)
	{
		return snake.GetSnakeNumber() == snakeNumber;
	});

	auto head = snake.GetSnakeHead();

	return (head.GetX() == i && head.GetY() == j);
}
