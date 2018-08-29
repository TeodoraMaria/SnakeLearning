#pragma once
#include "GameUtils.h"

class GameOptions
{
private:
	GameBoardType m_gameBoardType;
	size_t m_boardDimension;
	size_t m_numberOfPlayers;
	size_t m_foodPortions;

public:
	GameOptions(const GameBoardType& gameBoardType = GameBoardType::BOX, 
		const size_t& boardDimension = 10,
		const size_t& numberOfPlayers = 1,
		const size_t& foodPortions = 1
		);
	~GameOptions();
	GameBoardType GetGameBoardType() const;
	void SetGameBoardType(const GameBoardType& gameBoardType);
	size_t GetBoardDimension() const;
	void SetBoardDimension(const size_t& boardDimension);
	size_t GetNumberOfPlayers() const;
	void SetNumberOfPlayers(const size_t& numberOfPlayers);
	size_t GetFoodPortions() const;
	void SetFoodPortions(const size_t& foodPortions);
};