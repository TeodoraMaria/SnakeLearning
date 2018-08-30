#pragma once

#include "GameLogic/IPlayer.h"
#include <functional>

namespace GymEnv
{
	class SnakeStudent : public IPlayer
	{
	public:
		size_t GetSnakeNumber() const override;
		void SetSnakeNumber(const size_t& snakeNumber) override;
		
		SnakeMove GetNextAction(const GameState& gameState) const override;
		void SetNextAction(SnakeMove nextMove);
	
	private:
		size_t m_snakeNb;
		SnakeMove m_nextMove;
	};
}
