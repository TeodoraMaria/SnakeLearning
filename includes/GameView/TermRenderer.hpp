#pragma once

#include "GameLogic/GameState.h"

namespace GameView
{
	// To be moved in GameView somewhere somehow.
	
	class IGameRenderer
	{
	public:
		virtual void Render(const GameState& gameState) const = 0;
	};

	class TermRenderer : public IGameRenderer
	{
	public:
		void Render(const GameState& gameState) const override;
	};
}
