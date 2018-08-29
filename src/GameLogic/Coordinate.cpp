#include "Coordinate.h"

const Coordinate Coordinate::UP(-1, 0);
const Coordinate Coordinate::RIGHT(0, 1);
const Coordinate Coordinate::DOWN(1, 0);
const Coordinate Coordinate::LEFT(0, -1);

Coordinate::Coordinate(const size_t & x, const size_t & y): m_x(x),m_y(y)
{
}

Coordinate::~Coordinate()
{
}

size_t Coordinate::GetX() const
{
	return m_x;
}

size_t Coordinate::GetY() const
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

Coordinate Coordinate::Rotate90Left() const
{
	return Coordinate(-m_y,m_x);
}

Coordinate Coordinate::Rotate90Right() const
{
	return Coordinate(m_y, -m_x);
}

void Coordinate::GenerateCoordinate(const size_t& dimension)
{
	m_x = rand() % dimension;
	m_y = rand() % dimension;
}

bool Coordinate::CheckCoord(const std::vector<std::vector<int>>& board) const
{
	if (m_x < 0 || m_x >= board.size())
		return false;
	if (m_y < 0 || m_y >= board.size())
		return false;
	return board[m_x][m_y] == 0;
}