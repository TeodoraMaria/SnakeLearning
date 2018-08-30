#pragma once

#include "StepResult.h"
#include <cstddef>

namespace GymEnv
{
	/*
	** Env containers.
	*/
	
	struct SnakeGameState
	{
	};
	
	struct SnakeAction
	{
		int direction;
	};

	/*
	** The actual Gym env.
	*/

	class MultiSnakeEnv
	{
	public:
		MultiSnakeEnv(size_t nbOfSnakes = 1);
	
		SnakeGameState GetState() const;
		
		void Reset();
		void Render(); //what does render actually do?
		StepResult Step(const SnakeAction& action);
		
	private:
	};
}
