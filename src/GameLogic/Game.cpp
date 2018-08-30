#include <algorithm>
#include <stdlib.h>
#include <tuple>
#include <iostream>
#include <iomanip>
#include <windows.h>
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

std::vector<Snake> Game::GetLivingSnakes() const
{
	std::vector<Snake> livingSnakes(m_snakes.size());

	auto it = std::copy_if(m_snakes.begin(), m_snakes.end(), livingSnakes.begin(), [](const auto& snake) {return snake.IsAlive(); });
	livingSnakes.resize(std::distance(livingSnakes.begin(), it));

	return livingSnakes;
}

std::vector<Snake> Game::GetAllSnakes() const
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
	m_isGameOver = (GetLivingSnakes().size() == 0);
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

void Game::MoveSnake(const size_t & snakeNumber, const SnakeMove& move)
{
	auto& snakeToMove = *std::find_if(m_snakes.begin(), m_snakes.end(), 
		[snakeNumber](const auto& snake) 
		{
		return snake.GetSnakeNumber() == snakeNumber; 
		});
	if (snakeToMove.IsAlive())
	{
		Coordinate snakeOrientation = snakeToMove.GetOrientation();
		if (move == SnakeMove::LEFT)
		{
			snakeOrientation = snakeOrientation.Rotate90Left();
		}
		else if (move == SnakeMove::RIGHT)
		{
			snakeOrientation = snakeOrientation.Rotate90Right();
		}

		Coordinate newSnakeHeadPosition = snakeToMove.GetSnakeHead() + snakeOrientation;

		if (IsFood(newSnakeHeadPosition))
		{
			snakeToMove.Eat(newSnakeHeadPosition);
			m_gameBoard[newSnakeHeadPosition.GetX()][newSnakeHeadPosition.GetY()] = snakeNumber;
			PlaceFood();
		}
		else if (newSnakeHeadPosition.CheckCoord(m_gameBoard))
		{
			Coordinate freedPosition = snakeToMove.GetSnakeTail();
			m_gameBoard[freedPosition.GetX()][freedPosition.GetY()] = 0;
			snakeToMove.Move(newSnakeHeadPosition);
			m_gameBoard[newSnakeHeadPosition.GetX()][newSnakeHeadPosition.GetY()] = snakeNumber;
		}
		else {
			snakeToMove.Die(m_gameBoard);
		}
	}
}

void Game::RunRound()
{
	PrintBoard();
	std::random_shuffle(m_players.begin(), m_players.end());
	for (const auto& player : m_players)
	{
		auto chosenMove = player.GetNextAction(GameState(m_gameBoard, m_snakes));
		size_t snakeNumber = player.GetSnakeNumber();
		MoveSnake(snakeNumber, chosenMove);
	}
	std::cout << "All: " << GetAllSnakes().size() << "\nALive: " << GetLivingSnakes().size() << std::endl;
}

void Game::Play()
{
	while (!m_isGameOver)
	{
		RunRound();
		CheckIfGameOver();
	}
}

void Game::InitFood() 
{
	for (size_t foodIndex = 0; foodIndex < m_gameOptions.GetFoodPortions(); ++foodIndex)
	{
		PlaceFood();
	}
}

void Game::PlaceFood()
{
	Coordinate coord;
	do {
		coord.GenerateCoordinate(m_gameBoard.size());
	} while (!coord.CheckCoord(m_gameBoard));
	m_gameBoard[coord.GetX()][coord.GetY()] = 1;
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
