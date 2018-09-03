#pragma once

#include "StepResult.h"
#include "SnakeStudent.hpp"
#include "StateExtractor.hpp"
#include "GameLogic/Game.h"
#include "GameView/TermRenderer.hpp"
#include <cstddef>
#include <vector>

/*
** An environment where the snake sees one position ahead.
*/

namespace GymEnv
{
	class SingleSnakeGridView
	{
	public:
		static const std::vector<SnakeMove> actions;

		SingleSnakeGridView(
			size_t gridWidth,
			size_t gridHeight,
			GameView::IGameRenderer* gameRenderer);
		
		size_t GetNumbOfObservations() const;
		int GetState() const;
		
		void Reset();
		void Render();
		StepResult Step(SnakeMove moveDirection);
	
	private:
		std::unique_ptr<Game> m_game;
		std::shared_ptr<SnakeStudent> m_student;
		std::unique_ptr<GameView::IGameRenderer> m_gameRenderer;
	};
}
