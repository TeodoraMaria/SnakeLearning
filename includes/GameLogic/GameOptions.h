#pragma once
#include "GameUtils.h"
#include <cstddef>

class GameOptions
{
public:
	GameOptions(const GameBoardType& gameBoardType = GameBoardType::BOX, 
		const size_t& boardLength = 10,
		const size_t& boardWidth = 10,
		const size_t& numberOfPlayers = 1,
		const size_t& foodPortions = 1
		);
	~GameOptions();

	GameBoardType GetGameBoardType() const;
	size_t GetBoardLength() const;
	size_t GetBoardWidth() const;
	size_t GetNumberOfPlayers() const;
	size_t GetFoodPortions() const;

private:
	GameBoardType m_gameBoardType;
	size_t m_boardLength;
	size_t m_boardWidth;
	size_t m_numberOfPlayers;
	size_t m_foodPortions;
};
