#include "GameBoard.h"
#include <algorithm>

GameBoard::GameBoard(const GameBoardType & gameBoardType, const size_t & length, const size_t & width)
	:m_gameBoardType(gameBoardType), m_length(length), m_width(width)
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
		sum+= std::count_if(line.cbegin(), line.cend(), [](const auto& elem) {return elem == 1; });
	}
	return sum;
}

int& GameBoard::operator[](const Coordinate & coord)
{
	return m_board[coord.GetX()][coord.GetY()];
}

int GameBoard::operator[](const Coordinate & coord) const
{
	return m_board[coord.GetX()][coord.GetY()];
}

void GameBoard::Init()
{
	for (auto& line : m_board)
		for (auto& elem : line)
			elem = 0;

	if (!(m_gameBoardType == GameBoardType::LIMITLESS))
	{
		AddLimitsToBoard();
	}
}


void GameBoard::AddLimitsToBoard()
{
	for (size_t index = 0; index < m_width; ++index)
	{
		m_board[index][0] = -1;
		m_board[index][m_length-1] = -1;
	}

	for (size_t index = 0; index < m_length; ++index)
	{
		m_board[0][index] = -1;
		m_board[m_width-1][index] = -1;
	}
}

void GameBoard::PlaceFood()
{
	Coordinate coord;
	do {
		coord.GenerateCoordinate(m_width, m_length);
	} while (!CheckCoord(coord));
	(*this)[coord] = 1;
}

bool GameBoard::IsFood(const Coordinate & location)
{
	return (*this)[location] == 1;
}

void GameBoard::GrowSnake(const size_t & snakeNumber, const Coordinate & location)
{
	(*this)[location] = snakeNumber;
}

void GameBoard::MoveSnake(const Coordinate & freedLocation, const Coordinate & newLocation)
{
	(*this)[newLocation] = (*this)[freedLocation];
	(*this)[freedLocation] = 0;
}

void GameBoard::KillSnake(const std::list<Coordinate>& snakeBody)
{
	for (const auto& part : snakeBody)
	{
		(*this)[part] = 0;
	}
}

bool GameBoard::CheckCoord(const Coordinate& coord) const
{
	if (coord.GetX() < 0 || coord.GetX() >= m_width)
		return false;
	if (coord.GetY() < 0 || coord.GetY() >= m_length)
		return false;
	return (*this)[coord] == 0;
}

Coordinate& GameBoard::GenerateCoordinate() const
{
	Coordinate coord;
	do {
		coord.GenerateCoordinate(m_width, m_length);
	} while (!CheckCoord(coord));
	return coord;
}
