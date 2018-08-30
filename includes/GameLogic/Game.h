#pragma once
#include "HumanPlayer.h"
#include "Snake.h"
#include "GameOptions.h"

class Game
{
	typedef std::vector<std::vector<int>> GameBoard;

public:
	Game(const GameOptions& gameOptions, const std::vector<HumanPlayer>& players);
	~Game();

	GameBoard GetGameBoard() const;
	std::vector<Snake> GetAllSnakes() const;
	std::vector<Snake> GetLivingSnakes() const;

	void InitGame();
	void RunRound();
	void Play();

private:
	void AddLimitsToBoard();
	void InitSnakes();
	void CheckIfGameOver();
	void PrintBoard();
	void InitFood(const size_t& foodPortions = 0);
	void AddSnakeToGame(const size_t& snakeNumber);
	void MoveSnake(const size_t& snakeNumber, const SnakeMove& move);
	bool IsSnakeHead(const int& i, const int& j) const;
	bool IsFood(const Coordinate& location);


	GameBoard m_gameBoard;
	GameOptions m_gameOptions;
	std::vector<HumanPlayer> m_players;
	std::vector<Snake> m_snakes;
	bool m_isGameOver = false;
};
