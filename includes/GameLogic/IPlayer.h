#pragma once
#include "GameUtils.h"
#include "GameState.h"
#include <cstddef>
#include <memory>

class IPlayer
{
public:
	int GetSnakeNumber() const;
	void SetSnakeNumber(int snakeNumber);
	
	virtual SnakeMove GetNextAction(const GameState& gameState) const = 0;

private:
	int m_snakeNumber;
};

typedef std::shared_ptr<IPlayer> IPlayerPtr;
//typedef IPlayer* IPlayerPtr;
