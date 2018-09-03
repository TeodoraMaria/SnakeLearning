#pragma once
#include <vector>

class Coordinate
{
public:
	static const Coordinate UP;
	static const Coordinate RIGHT;
	static const Coordinate DOWN;
	static const Coordinate LEFT;

	Coordinate(const int& x = 0, const int& y = 0);
	~Coordinate();

	int GetX() const;
	int GetY() const;

	Coordinate operator + (const Coordinate& coord) const;
	Coordinate operator - (const Coordinate& coord) const;
	Coordinate operator * (const int scalar) const;
	bool operator == (const Coordinate& coord) const;

	Coordinate Rotate90Left() const;
	Coordinate Rotate90Right() const;

	void GenerateCoordinate(const int& width, const int& length);

private:
	int m_x;
	int m_y;
};