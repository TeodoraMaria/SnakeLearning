#pragma once
#include "HumanPlayer.h"
#include "Snake.h"
#include "GameOptions.h"
#include "GameBoard.h"
#include "FileHelper.h"
#include "GameView/IGameRenderer.hpp"
#include <string>

class Game
{
public:
	Game(const GameOptions& gameOptions, const std::vector<IPlayerPtr>& players);
	~Game();

	GameBoard GetGameBoard() const;
	std::vector<Snake> GetAllSnakes() const;
	std::vector<Snake> GetLivingSnakes() const;
	GameState GetGameState() const;
	size_t CountLivingSnakes() const;
	bool EveryoneIsDead() const;

	void InitGame();
	void RunRound();
	void Play(unsigned int maxNbOfSteps = std::numeric_limits<unsigned int>::max());
	
	void PrintBoard();
	void DisplayScoreBoard();
	int MoveSnake(const size_t& snakeNumber, const SnakeMove& move);
	void RestockFood();
	
	void ForcefullyKillPlayer(int snakeId);

private:
	void InitSnakes();
	void CheckIfGameOver() const;
	void InitFood();
	void AddSnakeToGame(const int snakeNumber);
	void DisablePlayer(const int snakeNumber);
	void SaveMove(const GameState& gameState, const SnakeMove& move, const int snakeNumber);
	bool IsSnakeHead(const Coordinate& coord) const;
	

	GameBoard m_gameBoard;
	GameOptions m_gameOptions;
	GameView::IGameRenderer* m_gameRenderer;
	std::vector<IPlayerPtr> m_players;
	std::vector<Snake> m_snakes;
	mutable bool m_isGameOver = false;
};
