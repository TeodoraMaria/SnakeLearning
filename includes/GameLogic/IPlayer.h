#pragma once
#include "GameUtils.h"
#include "GameState.h"
#include <cstddef>
#include <memory>

class IPlayer
{
public:
	static std::vector<SnakeMove> possibleMoves;

	int GetSnakeNumber() const;
	void SetSnakeNumber(int snakeNumber);
	bool GetIsActive() const;
	void SetIsActive(bool isActive);
	
	virtual SnakeMove GetNextAction(const GameState& gameState) const = 0;

private:
	int m_snakeNumber;
	bool m_isActive;
};

typedef std::shared_ptr<IPlayer> IPlayerPtr;
//typedef IPlayer* IPlayerPtr;
