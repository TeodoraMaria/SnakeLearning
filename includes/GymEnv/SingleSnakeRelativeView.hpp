#pragma once

#include "StepResult.h"
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

		size_t GetNumbOfObservations() const;
		int GetState() const;
		
		void Reset();
		void Render();
		StepResult Step(int moveDirection);
	};
}
