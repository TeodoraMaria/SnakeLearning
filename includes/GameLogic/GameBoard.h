#pragma once
#include "GameUtils.h"
#include "Coordinate.h"
#include <vector>
#include <list>

typedef std::vector<std::vector<int>> Board;

class GameBoard
{
public:
	GameBoard(const GameBoardType& gameBoardType = GameBoardType::BOX,
		const size_t& length = 10,
		const size_t& width = 10);
	~GameBoard();

	size_t GetBoardLength() const;
	size_t GetBoardWidth() const;
	size_t GetFoodPortions() const;

	int& operator [](const Coordinate& coord);
	int operator [](const Coordinate& coord) const;

	bool IsFood(const Coordinate& location);
	bool CheckCoord(const Coordinate& coord) const;
	bool CoordIsBounded(const Coordinate& coord) const;

	void Init();
	void PlaceFood();
	void GrowSnake(const size_t& snakeNumber, const Coordinate& location);
	void MoveSnake(const Coordinate& freedLocation, const Coordinate& newLocation);
	void KillSnake(const std::list<Coordinate>& snakeBody);
	Coordinate GenerateCoordinate() const;

private:
	void AddLimitsToBoard();
	
	Board m_board;
	size_t m_length;
	size_t m_width;
	GameBoardType m_gameBoardType;
};
