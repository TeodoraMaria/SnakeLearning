#pragma once
#include <vector>

class Coordinate
{
private:
	size_t m_x;
	size_t m_y;

public:
	static const Coordinate UP;
	static const Coordinate RIGHT;
	static const Coordinate DOWN;
	static const Coordinate LEFT;

	Coordinate(const size_t& x = 0, const size_t& y = 0);
	~Coordinate();

	size_t GetX() const;
	size_t GetY() const;

	void GenerateCoordinate(const size_t& dimension);
	bool CheckCoord(const std::vector<std::vector<int>>& board) const;
	
	Coordinate operator + (const Coordinate& coord) const;
	Coordinate operator - (const Coordinate& coord) const;
	Coordinate Rotate90Left() const;
	Coordinate Rotate90Right() const;
};