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
	m_players.resize(gameOptions.numberOfPlayers);
	m_gameBoard = GameBoard(
		gameOptions.gameBoardType,
		gameOptions.boardLength,
		gameOptions.boardWidth);
}

Game::~Game()
{
}

void Game::InitGame()
{
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

GameState Game::GetGameState() const
{
	return GameState(m_gameBoard, m_snakes);
}

std::vector<Snake> Game::GetAllSnakes() const
{
	return m_snakes;
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

void Game::SaveMove(FileHelper& helper, const std::vector<int> view, const SnakeMove& move)
{
	helper.WriteToFile(view, move);
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
	if (!m_gameOptions.playWithoutRenedring)
	{
		PrintBoard();
	}
	std::random_shuffle(m_players.begin(), m_players.end());
	for (auto player : m_players)
	{
		if (player->GetIsActive()==false)
			continue;
		
		const auto chosenMove = player->GetNextAction(GetGameState());

		const auto snakeNumber = player->GetSnakeNumber();
		MoveSnake(snakeNumber, chosenMove);
	}
	RestockFood();
}

void Game::RunRoundAndSave(FileHelper& helper)
{
	if (!m_gameOptions.playWithoutRenedring)
	{
		PrintBoard();
	}
	std::random_shuffle(m_players.begin(), m_players.end());
	for (auto player : m_players)
	{
		if (player->GetIsActive() == false)
			break;

		GameState gamestate = GetGameState();
		const auto chosenMove = player->GetNextAction(gamestate);
		const auto snakeNumber = player->GetSnakeNumber();
		SaveMove(helper, gamestate.GetFieldOfView(gamestate.GetSnake(snakeNumber),5,5), chosenMove);

		
		MoveSnake(snakeNumber, chosenMove);
	}
	RestockFood();
}

void Game::Play()
{
	if (m_gameOptions.saveGameplay)
	{
		FileHelper helper(GenerateFileName());
		while (!m_isGameOver)
		{
			RunRoundAndSave(helper);
			CheckIfGameOver();
		}
	}
	else 
	{
		while (!m_isGameOver)
		{
			RunRound();
			CheckIfGameOver();
		}
	}

	DisplayScoreBoard();
}

std::string Game::GenerateFileName()
{
	time_t now = time(0);

	tm *ltm = localtime(&now);

	std::string fileName = "D:\\SnakeData\\Game_";

	fileName.append(std::to_string(1900 + ltm->tm_year));
	fileName.append("_");
	fileName.append(std::to_string(1 + ltm->tm_mon));
	fileName.append("_");
	fileName.append(std::to_string(ltm->tm_mday));
	fileName.append("_");
	fileName.append(std::to_string(1 + ltm->tm_hour));
	fileName.append("_");
	fileName.append(std::to_string(1 + ltm->tm_min));
	fileName.append("_");
	fileName.append(std::to_string(1 + ltm->tm_sec));
	fileName.append(".csv");
	
	return fileName;
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
