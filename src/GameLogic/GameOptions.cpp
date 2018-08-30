#include "GameOptions.h"

GameOptions::GameOptions(const GameBoardType & gameBoardType, 
	const size_t & boardLength, 
	const size_t & boardLWidth,
	const size_t & numberOfPlayers, 
	const size_t & foodPortions)
	:m_gameBoardType(gameBoardType), m_boardLength(boardLength), m_boardWidth(boardLWidth),
	m_numberOfPlayers(numberOfPlayers), m_foodPortions(foodPortions)
{
}

GameOptions::~GameOptions()
{
}

GameBoardType GameOptions::GetGameBoardType() const
{
	return m_gameBoardType;
}

size_t GameOptions::GetBoardLength() const
{
	return m_boardLength;
}

size_t GameOptions::GetBoardWidth() const
{
	return m_boardWidth;
}

size_t GameOptions::GetNumberOfPlayers() const
{
	return m_numberOfPlayers;
}

size_t GameOptions::GetFoodPortions() const
{
	return m_foodPortions;
}