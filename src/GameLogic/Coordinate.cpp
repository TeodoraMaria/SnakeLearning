#include "Coordinate.h"
#include <stdlib.h>

const Coordinate Coordinate::UP(-1, 0);
const Coordinate Coordinate::RIGHT(0, 1);
const Coordinate Coordinate::DOWN(1, 0);
const Coordinate Coordinate::LEFT(0, -1);

Coordinate::Coordinate(const int & x, const int & y): m_x(x),m_y(y)
{
}

Coordinate::~Coordinate()
{
}

int Coordinate::GetX() const
{
	return m_x;
}

int Coordinate::GetY() const
{
	return m_y;
}

Coordinate Coordinate::operator+(const Coordinate & coord) const
{
	return Coordinate(m_x+coord.m_x, m_y+coord.m_y);
}

Coordinate Coordinate::operator-(const Coordinate & coord) const
{
	return Coordinate(m_x - coord.m_x, m_y - coord.m_y);
}

Coordinate Coordinate::operator*(const int scalar) const
{
	return Coordinate(m_x*scalar, m_y*scalar);
}

bool Coordinate::operator==(const Coordinate & coord) const
{
	if (m_x == coord.m_x)
		return m_y == coord.m_y;
	return false;
}

Coordinate Coordinate::Rotate90Left() const
{
	return Coordinate(-m_y,m_x);
}

Coordinate Coordinate::Rotate90Right() const
{
	return Coordinate(m_y, -m_x);
}

void Coordinate::GenerateCoordinate(const int& width, const int& length)
{
	m_x = rand() % width;
	m_y = rand() % length;
}

