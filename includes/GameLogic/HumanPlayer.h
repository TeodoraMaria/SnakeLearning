#pragma once
#include "IPlayer.h"

class HumanPlayer : public IPlayer
{
public:
	HumanPlayer();
	~HumanPlayer();

	size_t GetSnakeNumber() const override;

	void SetSnakeNumber(const size_t& snakeNumber) override;

	SnakeMove GetNextAction() const override;

private:
	size_t m_snakeNumber;
};