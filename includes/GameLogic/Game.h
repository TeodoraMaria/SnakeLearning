#pragma once
#include <vector>
#include <list>
#include "HumanPlayer.h"
#include "Snake.h"
#include "GameOptions.h"
#include "Coordinate.h"

class Game
{
	typedef std::vector<std::vector<int>> GameBoard;

private:
	GameBoard m_gameBoard;
	GameOptions m_gameOptions;
	std::vector<HumanPlayer> m_players;
	std::vector<Snake> m_snakes;

	bool m_isGameOver = false;
	
public:
	Game(const GameOptions& gameOptions, const std::vector<HumanPlayer>& players);
	~Game();

	GameBoard GetGameBoard() const;
	std::vector<Snake> GetSnakes() const;

	void InitGame();
	void RunRound();
	void Play();

private:
	void AddLimitsToBoard();
	void InitSnakes();
	void InitFood(const size_t& foodPortions = 0);
	void AddSnakeToGame(const size_t& snakeNumber);
	void MoveSnake(const size_t& snakeNumber, const SnakeMove& move);
	bool IsFood(const Coordinate& location);
	void CheckIfGameOver();
	void PrintBoard();
	bool IsSnakeHead(const int& i, const int& j) const;
};