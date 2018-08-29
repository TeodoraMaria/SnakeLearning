#pragma once

class IPlayer
{
public:
	virtual size_t GetSnakeNumber() { return 0; }

	virtual void SetSnakeNumber(size_t snakeNumber) {}
};