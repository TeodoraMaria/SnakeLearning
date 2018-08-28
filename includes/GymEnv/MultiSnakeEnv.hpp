#pragma once

namespace GymEnv
{
	/*
	** Env containers.
	*/
	
	struct StepResult
	{
		int reward = 0;
		bool isDone = false;
	};
	
	struct SnakeGameState
	{
	};
	
	struct SnakeGameAction
	{
	};

	/*
	** The actual Gym env.
	*/

	class MultiSnakeEnv
	{
	public:
		SnakeGameState GetState() const;
		
		void Reset();
		void Render();
		StepResult Step(const SnakeGameAction& action);
		
	private:
	};
}
