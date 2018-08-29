#pragma once
#include "IPlayer.h"

class HumanPlayer : public IPlayer
{
private:
	size_t m_snakeNumber;
public:
	HumanPlayer();
	~HumanPlayer();

	size_t GetSnakeNumber() const override;

	virtual void SetSnakeNumber(const size_t& snakeNumber) override;

	//0 for forward, 1 for left, 2 for right
	virtual int GetNextAction() const override;
};