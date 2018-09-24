#pragma once
#include "GameUtils.h"
#include "Coordinate.h"
#include <vector>
#include <list>
#include <cstddef>

static const int startOfSnakeIndexes = 10;

enum BoardPart
{
	EMPTY = 0,
	WALL = 1,
	FOOD = 2
};

enum class GameBoardType
{
	LIMITLESS,
	BOX,
	CUSTOM
};

class GameBoard
{
public:
	static bool IsObstacle(BoardPart boardPart);

	GameBoard(
		const GameBoardType& gameBoardType = GameBoardType::BOX,
		const size_t& length = 10,
		const size_t& width = 10);
	~GameBoard();

	size_t GetBoardLength() const;
	size_t GetBoardWidth() const;
	size_t GetFoodPortions() const;
	std::vector<int> GetBoard() const;
	const std::vector<int>& GetBoardCells() const;

	int& operator [](const Coordinate& coord);
	int operator [](const Coordinate& coord) const;

	bool IsFood(const Coordinate& location) const;
	bool CoordIsEmpty(const Coordinate& coord) const;
	bool CoordIsBounded(const Coordinate& coord) const;
	bool IsWallOrBeyond(const Coordinate& coord) const;
	bool HasFreeSpace() const;

	void Init();
	void PlaceFood();
	void RemoveFood(const Coordinate& location);
	void GrowSnake(const size_t& snakeNumber, const Coordinate& location);
	void MoveSnake(const Coordinate& freedLocation, const Coordinate& newLocation);
	void KillSnake(const std::list<Coordinate>& snakeBody);
	Coordinate GenerateCoordinate() const;

	Coordinate FindClosestFood(const Coordinate& position) const;
    const std::vector<int>& GetBoardCells() const;

private:
	
	void AddLimitsToBoard();
	
	std::vector<int> m_board;
	std::vector<Coordinate> m_foods;
	
	size_t m_length;
	size_t m_width;
	GameBoardType m_gameBoardType;
};
