#pragma once
#include <vector>

struct Coordinate {
	int lineCoord;
	int colCoord;

	void GenerateCoordinate(const size_t& dimension) 
	{
		lineCoord = rand() % dimension;
		colCoord = rand() % dimension;
	}

	bool CheckCoord(const std::vector<std::vector<int>>& board) const
	{
		if (lineCoord < 0 || lineCoord >= board.size())
			return false;
		if (colCoord < 0 || colCoord >= board.size())
			return false;
		return board[lineCoord][colCoord] == 0;
	}
};

enum class GameBoardType {LIMITLESS, BOX, CUSTOM};