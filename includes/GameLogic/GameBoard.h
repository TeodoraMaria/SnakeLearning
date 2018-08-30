#pragma once
#include "GameUtils.h"
#include "Coordinate.h"
#include <vector>

typedef std::vector<std::vector<int>> Board;

class GameBoard
{
public:
	GameBoard(const GameBoardType& gameBoardType = GameBoardType::BOX,
		const size_t& length = 10,
		const size_t& width = 10);
	~GameBoard();

	Board& GetBoard();
	size_t GetBoardLength() const;
	size_t GetBoardWidth() const;

	int& operator [](const Coordinate& coord);
	int operator [](const Coordinate& coord) const;

	void Init();

private:
	void AddLimitsToBoard();

	Board m_board;
	size_t m_length;
	size_t m_width;
	GameBoardType m_gameBoardType;
};