#pragma once

#include "IGameRenderer.hpp"

namespace GameView
{
	class TermRenderer : public IGameRenderer
	{
	public:
		void Render(const GameState& gameState) const override;
	};
}
