#pragma once
#include "GameUtils.h"

class GameOptions
{
public:
	GameOptions(const GameBoardType& gameBoardType = GameBoardType::BOX, 
		const size_t& boardDimension = 10,
		const size_t& numberOfPlayers = 1,
		const size_t& foodPortions = 1
		);
	~GameOptions();

	GameBoardType GetGameBoardType() const;
	size_t GetBoardDimension() const;
	size_t GetNumberOfPlayers() const;
	size_t GetFoodPortions() const;

private:
	GameBoardType m_gameBoardType;
	size_t m_boardDimension;
	size_t m_numberOfPlayers;
	size_t m_foodPortions;
};