#pragma once
#include "GameUtils.h"
#include "GameState.h"
#include <cstddef>
#include <memory>

class IPlayer
{
public:
	virtual size_t GetSnakeNumber() const = 0;
	virtual void SetSnakeNumber(const size_t& snakeNumber) = 0;
	virtual SnakeMove GetNextAction(const GameState& gameState) const = 0;
};

typedef std::shared_ptr<IPlayer> IPlayerPtr;
//typedef IPlayer* IPlayerPtr;
