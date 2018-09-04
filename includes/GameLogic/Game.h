#pragma once
#include "HumanPlayer.h"
#include "Snake.h"
#include "GameOptions.h"
#include "GameBoard.h"

class Game
{
public:
	Game(const GameOptions& gameOptions, const std::vector<IPlayerPtr>& players);
	~Game();

	GameBoard GetGameBoard() const;
	std::vector<Snake> GetAllSnakes() const;
	std::vector<Snake> GetLivingSnakes() const;
	GameState GetGameState() const;

	void InitGame();
	void RunRound();
	void Play();
	
	void PrintBoard();
	void DisplayScoreBoard();
	int MoveSnake(const size_t& snakeNumber, const SnakeMove& move);
	void RestockFood();

private:
	void InitSnakes();
	void CheckIfGameOver();
	void InitFood();
	void AddSnakeToGame(const int snakeNumber);
	void DisablePlayer(const int snakeNumber);
	bool IsSnakeHead(const Coordinate& coord) const;

	

	GameBoard m_gameBoard;
	GameOptions m_gameOptions;
	std::vector<IPlayerPtr> m_players;
	std::vector<Snake> m_snakes;
	bool m_isGameOver = false;
};
