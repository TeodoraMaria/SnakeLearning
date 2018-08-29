#include "..\..\includes\GameLogic\GameOptions.h"

GameOptions::GameOptions(const GameBoardType & gameBoardType, 
	const size_t & boardDimension, 
	const size_t & numberOfPlayers, 
	const size_t & foodPortions)
	:m_gameBoardType(gameBoardType), m_boardDimension(boardDimension),
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

void GameOptions::SetGameBoardType(const GameBoardType & gameBoardType)
{
	m_gameBoardType = gameBoardType;
}

size_t GameOptions::GetBoardDimension() const
{
	return m_boardDimension;
}

void GameOptions::SetBoardDimension(const size_t & boardDimension)
{
	m_boardDimension = boardDimension;
}

size_t GameOptions::GetNumberOfPlayers() const
{
	return m_numberOfPlayers;
}

void GameOptions::SetNumberOfPlayers(const size_t & numberOfPlayers)
{
	m_numberOfPlayers = numberOfPlayers;
}

size_t GameOptions::GetFoodPortions() const
{
	return m_foodPortions;
}

void GameOptions::SetFoodPortions(const size_t & foodPortions)
{
	m_foodPortions = foodPortions;
}
