#include "Game.h"
#include "MultiPlatformTerm.hpp"
#include <algorithm>
#include <stdlib.h>
#include <tuple>
#include <iostream>
#include <iomanip>

#ifdef _WIN32
	#include <windows.h>
#endif

Game::Game(
	const GameOptions& gameOptions,
	const std::vector<IPlayerPtr>& players) :
	m_gameOptions(gameOptions),
	m_players(players)
{
	m_players.resize(gameOptions.GetNumberOfPlayers());
	m_gameBoard = GameBoard(
		gameOptions.GetGameBoardType(),
		gameOptions.GetBoardLength(),
		gameOptions.GetBoardWidth());
}

Game::~Game()
{
}

void Game::InitGame()
{
	m_gameBoard.Init();
	InitSnakes();
	InitFood();
}

GameBoard Game::GetGameBoard() const
{
	return m_gameBoard;
}

std::vector<Snake> Game::GetLivingSnakes() const
{
	std::vector<Snake> livingSnakes(m_snakes.size());

	auto it = std::copy_if(
		m_snakes.begin(),
		m_snakes.end(),
		livingSnakes.begin(), [](const auto& snake) {return snake.IsAlive(); });

	livingSnakes.resize(std::distance(livingSnakes.begin(), it));

	return livingSnakes;
}

std::vector<Snake> Game::GetAllSnakes() const
{
	return m_snakes;
}


void Game::InitSnakes()
{
	size_t snakeIndex = 10;
	for (auto player : m_players)
	{
		++snakeIndex;
		AddSnakeToGame(snakeIndex);
		player->SetSnakeNumber(snakeIndex);
	}
}

void Game::AddSnakeToGame(const size_t& snakeNumber)
{
	Snake snake(snakeNumber);
	snake.InitSnake(m_gameBoard);
	m_snakes.push_back(snake);
}

void Game::CheckIfGameOver()
{
	m_isGameOver = (GetLivingSnakes().size() == 0);
}

#ifdef _WIN32
	void Game::PrintBoard()
	{
		HANDLE  hConsole;
		int k;

		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		for (size_t i = 0; i<m_gameBoard.GetBoardWidth(); i++)
		{
			for (size_t j = 0; j<m_gameBoard.GetBoardLength(); j++)
			{
				k = IsSnakeHead(Coordinate(i, j)) ? 12 : 15;
				SetConsoleTextAttribute(hConsole, k);
				std::cout << std::setw(4) << m_gameBoard[Coordinate(i, j)] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
#else
	void Game::PrintBoard()
	{
		for (size_t i = 0; i<m_gameBoard.GetBoardWidth(); i++)
		{
			for (size_t j = 0; j<m_gameBoard.GetBoardLength(); j++)
			{
				const auto targetPrint = m_gameBoard[Coordinate(i, j)];
				if (IsSnakeHead(Coordinate(i, j)))
				{
					std::cout << "  ";
					MultiPlatform::PrintColoredStr(
						MultiPlatform::Color::RED,
						std::to_string(targetPrint));
				}
				else
					std::cout << std::setw(4) << targetPrint;
				
				std::cout << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
#endif

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

		const auto newSnakeHeadPosition =
			snakeToMove.GetSnakeHead() + snakeOrientation;

		if (m_gameBoard.IsFood(newSnakeHeadPosition))
		{
			snakeToMove.Eat(newSnakeHeadPosition);
			m_gameBoard[newSnakeHeadPosition] = snakeNumber;
			m_gameBoard.PlaceFood();
		}
		else if (m_gameBoard.CheckCoord(newSnakeHeadPosition))
		{
			Coordinate freedPosition = snakeToMove.GetSnakeTail();
			snakeToMove.Move(newSnakeHeadPosition);
			m_gameBoard.MoveSnake(freedPosition, newSnakeHeadPosition);
		}
		else {
			m_gameBoard.KillSnake(snakeToMove.GetSnakeBody());
			snakeToMove.Die();
		}
	}
}

void Game::RunRound()
{
	PrintBoard();
	std::random_shuffle(m_players.begin(), m_players.end());
	for (auto player : m_players)
	{
		const auto chosenMove =
			player->GetNextAction(GameState(m_gameBoard, m_snakes));
		
		const auto snakeNumber = player->GetSnakeNumber();
		MoveSnake(snakeNumber, chosenMove);
	}
	std::cout
		<< "All: " << GetAllSnakes().size() << std::endl
		<< "Alive: " << GetLivingSnakes().size()
		<< std::endl;
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
	for (auto i = 0u; i < m_gameOptions.GetFoodPortions(); ++i)
	{
		m_gameBoard.PlaceFood();
	}
}

bool Game::IsSnakeHead(const Coordinate& coord) const
{
	if (m_gameBoard[coord] <= 10)
		return false;
	
	auto snakeNumber = m_gameBoard[coord];
	auto snake = *std::find_if(
		m_snakes.begin(),
		m_snakes.end(),
		[snakeNumber](const auto& snake)
	{
		return snake.GetSnakeNumber() == snakeNumber;
	});

	auto head = snake.GetSnakeHead();

	return (head == coord);
}
