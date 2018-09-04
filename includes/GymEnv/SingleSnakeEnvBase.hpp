#pragma once

#include "SnakeEnvBase.hpp"
#include "StepResult.h"
#include "SnakeStudent.hpp"

namespace GymEnv
{
	class SingleSnakeEnvBase : public SnakeEnvBase
	{
	public:
		SingleSnakeEnvBase(
			GameView::IGameRenderer* gmRenderer,
			const GameOptions& gmOptions);
		
		virtual size_t GetNumbOfObservations() const = 0;
		virtual const std::vector<int> GetState() const = 0;
		
		void Reset();
		StepResult Step(SnakeMove moveDirection);
	
	protected:
		std::shared_ptr<SnakeStudent> m_student;
	};
}
