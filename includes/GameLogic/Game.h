#pragma once
#include <vector>
#include <list>
#include "IPlayer.h"
#include "Snake.h"

class Game
{
	typedef std::vector<std::vector<int>> GameBoard;
private:
	GameBoard m_gameBoard;
	std::vector<IPlayer> m_players;
	std::vector<Snake> m_snakes;

	bool m_isLimitless;
	size_t m_numberOfFoods;

	void AddLimitsToBoard();
	void InitSnakes();
	void InitFood();
	void AddSnakeToGame(size_t snakeNumber);
public:
	Game(bool limitless = false, size_t boardDimension = 10, size_t numberOfPlayers = 1, size_t foodNo = 1);
	~Game();
	void InitGame();
	GameBoard GetGameBoard() const;
	std::vector<Snake> GetSnakes() const;
};