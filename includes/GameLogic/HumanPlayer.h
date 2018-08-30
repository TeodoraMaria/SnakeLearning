#pragma once
#include "IPlayer.h"

class HumanPlayer : public IPlayer
{
public:
	HumanPlayer();
	~HumanPlayer();

	size_t GetSnakeNumber() const override;

	void SetSnakeNumber(const size_t& snakeNumber) override;

	SnakeMove GetNextAction(const GameState& gameState) const override;

private:
	SnakeMove GetUserInput() const;
	void TranslateMoveToSnakeView(SnakeMove& move, const GameState& gameState) const;
	void RotateLeft(SnakeMove& move) const;

	size_t m_snakeNumber;
};