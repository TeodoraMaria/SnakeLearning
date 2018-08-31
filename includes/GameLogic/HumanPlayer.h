#pragma once
#include "IPlayer.h"

class HumanPlayer : public IPlayer
{
public:
	HumanPlayer();
	~HumanPlayer();

	SnakeMove GetNextAction(const GameState& gameState) const override;

private:
	SnakeMove GetUserInput() const;
	void TranslateMoveToSnakeView(SnakeMove& move, const GameState& gameState) const;
	void RotateLeft(SnakeMove& move) const;
};
