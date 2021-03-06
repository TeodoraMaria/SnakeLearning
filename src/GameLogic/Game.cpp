#include "Game.h"
#include "FileHelper.h"
#include "MultiplatformUtils/Getch.h"
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
	m_players.resize(players.size());
	m_gameBoard = GameBoard(
		gameOptions.gameBoardType,
		gameOptions.boardLength,
		gameOptions.boardWidth);
	m_gameRenderer = gameOptions.gameRenderer;
}

Game::~Game()
{
}

void Game::InitGame()
{
//	srand(3);
	m_isGameOver = false;
	m_snakes.clear();
	for (auto& player : m_players)
		player->SetIsActive(true);
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

size_t Game::CountLivingSnakes() const
{
	return std::count_if(m_snakes.cbegin(), m_snakes.cend(),
		[](const auto& snake) { return snake.IsAlive(); });
}

GameState Game::GetGameState() const
{
	return GameState(m_gameBoard, m_snakes);
}

std::vector<Snake> Game::GetAllSnakes() const
{
	return m_snakes;
}

bool Game::EveryoneIsDead() const
{
	CheckIfGameOver();
	return m_isGameOver;
}

void Game::ForcefullyKillPlayer(int snakeId)
{
	auto& snake = *std::find_if(m_snakes.begin(), m_snakes.end(),
		[&](const auto& snake)
		{
			return snake.GetSnakeNumber() == snakeId;
		});
	
	m_gameBoard.KillSnake(snake.GetSnakeBody());
	snake.Die();
	DisablePlayer(snakeId);
}

void Game::InitSnakes()
{
	size_t snakeIndex = startOfSnakeIndexes;
	for (auto player : m_players)
	{
		++snakeIndex;
		AddSnakeToGame(snakeIndex);
		player->SetSnakeNumber(snakeIndex);
	}
}

void Game::AddSnakeToGame(const int snakeNumber)
{
	Snake snake(snakeNumber);
	
	snake.InitSnake(m_gameBoard, m_gameOptions.initialSnakeSize);
	m_snakes.push_back(snake);
}

void Game::DisablePlayer(const int snakeNumber)
{
	auto it = std::find_if(m_players.begin(), m_players.end(), 
		[snakeNumber](const auto& player) {return player->GetSnakeNumber() == snakeNumber; });
	(*it)->SetIsActive(false);
}

void Game::SaveMove(const GameState& gameState, const SnakeMove& move, const int snakeNumber)
{
	FileHelper helper(m_gameOptions.gameplayLog);
	const std::vector<int> view = gameState.GetMap();
	const auto snakeHead = gameState.GetSnake(snakeNumber).GetSnakeHead();
	int snakeHeadPos = gameState.GetGameBoard().GetBoardLength()*snakeHead.GetX() + snakeHead.GetY();
	int boardLength = gameState.GetGameBoard().GetBoardLength();
	const auto snakeNeck = gameState.GetSnake(snakeNumber).GetSnakeNeck();
	int snakeNeckPos = gameState.GetGameBoard().GetBoardLength()*snakeNeck.GetX() + snakeNeck.GetY();
	GameplayStep step(boardLength, view, move, snakeHeadPos, snakeNeckPos);
	helper.WriteToFile(step);
}

void Game::CheckIfGameOver() const
{
	m_isGameOver = (CountLivingSnakes() == 0);
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
					std::cout << "  \033[31m" << targetPrint << "\033[0m";
				else
					std::cout << std::setw(4) << targetPrint;
				
				std::cout << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
#endif

void Game::DisplayScoreBoard()
{ 
	std::sort(m_snakes.begin(), m_snakes.end(),
		[](const auto& snakeOne, const auto& snakeTwo)
		{
			return snakeOne.GetScore() > snakeTwo.GetScore();
		});

	std::cout << "\nSCORE BOARD:\n";
	for (const auto& snake : m_snakes) 
	{
		std::cout <<std::setw(4)<< snake.GetSnakeNumber() << " - " << snake.GetScore()<<std::endl;
	}
}

int Game::MoveSnake(const size_t & snakeNumber, const SnakeMove& move)
{
	auto& snakeToMove = *std::find_if(m_snakes.begin(), m_snakes.end(), 
		[snakeNumber](const auto& snake) 
		{
			return snake.GetSnakeNumber() == snakeNumber;
		});
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
		m_gameBoard.RemoveFood(newSnakeHeadPosition);
		snakeToMove.Eat(newSnakeHeadPosition);
		m_gameBoard[newSnakeHeadPosition] = snakeNumber;
		return 1;
	}
	else if (m_gameBoard.CoordIsEmpty(newSnakeHeadPosition) || newSnakeHeadPosition== snakeToMove.GetSnakeTail())
	{
		Coordinate freedPosition = snakeToMove.GetSnakeTail();
		snakeToMove.Move(newSnakeHeadPosition);
		m_gameBoard.MoveSnake(freedPosition, newSnakeHeadPosition);
		return 0;
	}
    
	else 
	{
		m_gameBoard.KillSnake(snakeToMove.GetSnakeBody());
		snakeToMove.Die();
		DisablePlayer(snakeNumber);
		return -1;
	}
    
}

void Game::RestockFood()
{
	while (m_gameBoard.GetFoodPortions() < m_gameOptions.numFoods && m_gameBoard.HasFreeSpace())
	{
		m_gameBoard.PlaceFood();
	}
}

void Game::RunRound()
{
	GameState gameState = GetGameState();
	if (m_gameRenderer != nullptr)
	{
//		PrintBoard();
		m_gameRenderer->Render(GetGameState());
	}
	std::random_shuffle(m_players.begin(), m_players.end());
	for (auto player : m_players)
	{
		if (player->GetIsActive()==false)
			continue;
		
		const auto chosenMove = player->GetNextAction(gameState);

		const auto snakeNumber = player->GetSnakeNumber();
		if (m_gameOptions.gameplayLog != "")
		{
			SaveMove(gameState, chosenMove, snakeNumber);
		}
		MoveSnake(snakeNumber, chosenMove);
	}
	RestockFood();
}

void Game::Play(unsigned int maxNbOfSteps)
{
	for (auto i = 0u; i < maxNbOfSteps && !m_isGameOver; i++)
	{
		RunRound();
		CheckIfGameOver();
	}

	DisplayScoreBoard();
}

void Game::InitFood() 
{
	for (auto i = 0u; i < m_gameOptions.numFoods; ++i)
	{
		m_gameBoard.PlaceFood();
	}
}

bool Game::IsSnakeHead(const Coordinate& coord) const
{
	if (m_gameBoard[coord] <= startOfSnakeIndexes)
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
