#pragma once

#include "GameLogic/IPlayer.h"

namespace AI { namespace HardCoded
{
	class SingleBot : public IPlayer
	{
	public:
		size_t GetSnakeNumber() const override;
		void SetSnakeNumber(const size_t& snakeNumber) override;
		
		SnakeMove GetNextAction(const GameState& gameState) const override;
	
	private:
		size_t m_snakeNb;
	};
}}