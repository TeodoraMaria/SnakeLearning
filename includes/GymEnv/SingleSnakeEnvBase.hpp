#pragma once

#include "SnakeEnvBase.hpp"
#include "StepResult.h"
#include "SnakeStudent.hpp"
#include "GameLogic/CellInterpreter/ICellInterpreter.hpp"

namespace GymEnv
{
	struct SingleSnakeEnvBaseModel
	{
		GameView::IGameRenderer* gmRenderer;
		const GameOptions* gmOptions;
		GameLogic::CellInterpreter::ICellInterpreterPtr celInterpreter;
	};

	class SingleSnakeEnvBase : public SnakeEnvBase
	{
	public:
		SingleSnakeEnvBase(const SingleSnakeEnvBaseModel& model);
		
		GameLogic::CellInterpreter::ICellInterpreterPtr GetCellInterpreter();
		virtual size_t GetNumbOfObservations() const = 0;
		virtual const std::vector<double> GetState() const = 0;
		
		void Reset();
		StepResult Step(SnakeMove moveDirection);
	
	protected:
		GameLogic::CellInterpreter::ICellInterpreterPtr m_celInterpreter;
		std::shared_ptr<SnakeStudent> m_student;
	};
}
