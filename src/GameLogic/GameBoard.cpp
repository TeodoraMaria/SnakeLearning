#include "GameBoard.h"
#include <algorithm>

/*
** Constructors & Destructors.
*/

GameBoard::GameBoard(
	const GameBoardType& gameBoardType,
	const size_t& length,
	const size_t& width) :
	m_length(length),
	m_width(width),
	m_gameBoardType(gameBoardType)
{
	m_board.resize(m_width);
	for (size_t index = 0; index < m_width; ++index)
	{
		m_board[index].resize(m_length);
	}
}

GameBoard::~GameBoard()
{
}

/*
** Getters.
*/

size_t GameBoard::GetBoardLength() const
{
	return m_length;
}

size_t GameBoard::GetBoardWidth() const
{
	return m_width;
}

size_t GameBoard::GetFoodPortions() const
{
	size_t sum = 0;
	for (const auto& line : m_board)
	{
		sum += std::count_if(
			line.cbegin(),
			line.cend(),
			[](const auto& elem) { return elem == BoardPart::FOOD; });
	}
	return sum;
}

/*
** Operators.
*/

int& GameBoard::operator[](const Coordinate & coord)
{
	return m_board[coord.GetX()][coord.GetY()];
}

int GameBoard::operator[](const Coordinate & coord) const
{
	return m_board[coord.GetX()][coord.GetY()];
}

/*
** Bool states.
*/

bool GameBoard::IsFood(const Coordinate & location) const
{
	return (*this)[location] == BoardPart::FOOD;
}

bool GameBoard::CoordIsEmpty(const Coordinate& coord) const
{
	return  CoordIsBounded(coord) && (*this)[coord] == BoardPart::EMPTY;
}

bool GameBoard::IsWallOrBeyond(const Coordinate& coord) const
{
	return !CoordIsBounded(coord) || (*this)[coord] == BoardPart::WALL;
}

bool GameBoard::HasFreeSpace() const
{
	for (const auto& line : m_board)
		for (const auto& elem : line)
			if (elem == 0)
				return true;
	return false;
}

bool GameBoard::CoordIsBounded(const Coordinate& coord) const
{
	if (coord.GetX() < 0 || coord.GetX() >= static_cast<int>(m_width))
		return false;
	if (coord.GetY() < 0 || coord.GetY() >= static_cast<int>(m_length))
		return false;
	return true;
}

/*
** Other public methods.
*/

void GameBoard::Init()
{
	for (auto& line : m_board)
		for (auto& elem : line)
			elem = BoardPart::EMPTY;

	if (!(m_gameBoardType == GameBoardType::LIMITLESS))
	{
		AddLimitsToBoard();
	}
}

void GameBoard::AddLimitsToBoard()
{
	for (size_t index = 0; index < m_width; ++index)
	{
		m_board[index][0] = BoardPart::WALL;
		m_board[index][m_length - 1] = BoardPart::WALL;
	}

	for (size_t index = 0; index < m_length; ++index)
	{
		m_board[0][index] = BoardPart::WALL;
		m_board[m_width - 1][index] = BoardPart::WALL;
	}
}

void GameBoard::PlaceFood()
{
	Coordinate coord;
	do {
		coord.GenerateCoordinate(m_width, m_length);
	} while (!CoordIsEmpty(coord));
	(*this)[coord] = BoardPart::FOOD;
}

void GameBoard::GrowSnake(const size_t & snakeNumber, const Coordinate & location)
{
	(*this)[location] = snakeNumber;
}

void GameBoard::MoveSnake(const Coordinate & freedLocation, const Coordinate & newLocation)
{
	(*this)[newLocation] = (*this)[freedLocation];
	(*this)[freedLocation] = BoardPart::EMPTY;
}

void GameBoard::KillSnake(const std::list<Coordinate>& snakeBody)
{
	for (const auto& part : snakeBody)
	{
		(*this)[part] = BoardPart::EMPTY;
	}
}

Coordinate GameBoard::GenerateCoordinate() const
{
	Coordinate coord;
	do {
		coord.GenerateCoordinate(m_width, m_length);
	} while (!CoordIsEmpty(coord));
	return coord;
}
