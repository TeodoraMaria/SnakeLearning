#pragma once

#include "StepResult.h"
#include "SnakeEnvBase.hpp"
#include "SnakeAgent.hpp"

namespace GymEnv
{
	struct MultiSnakeEnvModel
	{
		GameView::IGameRenderer* gameRenderer = nullptr;
		const GameOptions* gmOptions;
		std::vector<std::shared_ptr<SnakeAgent>> agents;
	};

	class MultiSnakeEnv : public SnakeEnvBase
	{
	public:
		MultiSnakeEnv(const MultiSnakeEnvModel& model);
		
		const GameState GetState() const;
		
		void Reset();
		StepResult Step(int snakeId, SnakeMove move);
	
	private:
		std::vector<std::shared_ptr<SnakeAgent>> m_agents;
	};
}
