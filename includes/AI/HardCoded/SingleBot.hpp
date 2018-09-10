#pragma once

#include "GameLogic/IPlayer.h"

namespace AI { namespace HardCoded
{
	class SingleBot : public IPlayer
	{
	public:
		SnakeMove GetNextAction(const GameState& gameState) override;
	};
}}
