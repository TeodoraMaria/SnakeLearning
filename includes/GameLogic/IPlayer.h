#pragma once

class IPlayer
{
public:
	virtual size_t GetSnakeNumber() const = 0;

	virtual void SetSnakeNumber(const size_t& snakeNumber) = 0;

	virtual int GetNextAction() const = 0;
};