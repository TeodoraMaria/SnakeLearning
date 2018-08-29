#pragma once
#include <vector>
#include <list>
#include "IPlayer.h"
#include "Snake.h"
#include "GameOptions.h"

class Game
{
	typedef std::vector<std::vector<int>> GameBoard;
private:
	GameBoard m_gameBoard;
	GameOptions m_gameOptions;
	std::vector<IPlayer> m_players;
	std::vector<Snake> m_snakes;

	void AddLimitsToBoard();
	void InitSnakes();
	void InitFood();
	void AddSnakeToGame(const size_t& snakeNumber);
public:
	Game(const GameOptions& gameOptions);
	~Game();
	void InitGame();
	GameBoard GetGameBoard() const;
	std::vector<Snake> GetSnakes() const;
};