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

	void InitGame();
	void RunRound();
	void Play();
	void MoveSnake(const size_t& snakeNumber, const SnakeMove& move);

private:
	void InitSnakes();
	void CheckIfGameOver();
	void PrintBoard();
	void InitFood();
	void RestockFood();
	void AddSnakeToGame(const size_t& snakeNumber);
	bool IsSnakeHead(const Coordinate& coord) const;
	

	GameBoard m_gameBoard;
	GameOptions m_gameOptions;
	std::vector<IPlayerPtr> m_players;
	std::vector<Snake> m_snakes;
	bool m_isGameOver = false;
};
