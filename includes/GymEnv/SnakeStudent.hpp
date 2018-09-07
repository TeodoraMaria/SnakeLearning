#pragma once

#include "GameLogic/IPlayer.h"
#include <functional>

namespace GymEnv
{
	class SnakeStudent : public IPlayer
	{
	public:
		SnakeMove GetNextAction(const GameState& gameState) override;
		void SetNextAction(SnakeMove nextMove);
	
	private:
		SnakeMove m_nextMove;
	};
}
