#pragma once

#include "StepResult.h"
#include "SnakeStudent.hpp"
#include "GameLogic/Game.h"
#include <cstddef>
#include <array>
#include <vector>

/*
** An environment where the snake sees one position ahead.
*/

namespace GymEnv
{
	class SingleSnakeRelativeView
	{
	public:
		static const std::array<int, 3> actions;

		SingleSnakeRelativeView();
		
		size_t GetNumbOfObservations() const;
		int GetState() const;
		
		void Reset();
		void Render();
		StepResult Step(SnakeMove moveDirection);
	
	private:
		std::unique_ptr<Game> m_game;
		std::shared_ptr<SnakeStudent> m_student;
	};
}
