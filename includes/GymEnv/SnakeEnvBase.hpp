#pragma once

#include "GameLogic/Game.h"
#include "GameLogic/GameUtils.h"
#include "GameView/IGameRenderer.hpp"
#include <vector>

namespace GymEnv
{
	class SnakeEnvBase
	{
	public:
		static const std::vector<SnakeMove> actions;
		
		const Game& GetGame() const;
		
		SnakeEnvBase(GameView::IGameRenderer* gameRenderer = nullptr);
		void Render() const;
	
	protected:
		std::unique_ptr<Game> m_game;
	
	private:
		GameView::IGameRenderer* m_gameRenderer = nullptr;
	};
}
