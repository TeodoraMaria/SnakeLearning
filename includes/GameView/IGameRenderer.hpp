#pragma once

#include "GameLogic/GameState.h"

namespace GameView
{
	class IGameRenderer
	{
	public:
		virtual void Render(const GameState& gameState) const = 0;
	};
}
