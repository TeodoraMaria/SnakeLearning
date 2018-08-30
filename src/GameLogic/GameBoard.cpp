#include "GameBoard.h"

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

Board& GameBoard::GetBoard() 
{
	return m_board;
}

size_t GameBoard::GetBoardLength() const
{
	return m_length;
}

size_t GameBoard::GetBoardWidth() const
{
	return m_width;
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
