#pragma once
#include <vector>

struct Coordinate {
	int lineCoord;
	int colCoord;

	enum Direction { UP, DOWN, LEFT, RIGHT };

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

	Coordinate operator + (const Direction& direction)
	{
		Coordinate coordinate = *this;
		switch (direction)
		{
		case UP:
			coordinate.lineCoord--;
			break;
		case DOWN:
			coordinate.lineCoord++;
			break;
		case LEFT:
			coordinate.colCoord--;
			break;
		case RIGHT:
			coordinate.colCoord++;
			break;
		}
		return coordinate;
	}
};

enum class GameBoardType {LIMITLESS, BOX, CUSTOM};