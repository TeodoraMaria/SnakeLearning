#pragma once
#include "GameUtils.h"

class IPlayer
{
public:
	virtual size_t GetSnakeNumber() const = 0;

	virtual void SetSnakeNumber(const size_t& snakeNumber) = 0;

	virtual SnakeMove GetNextAction() const = 0;
};