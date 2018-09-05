#pragma once

#include "SingleSnakeEnvBase.hpp"
#include "StepResult.h"
#include "SnakeStudent.hpp"
#include "StateExtractor.hpp"
#include "GameView/IGameRenderer.hpp"
#include <cstddef>
#include <vector>

/*
** An environment where the snake sees one position ahead.
*/

namespace GymEnv
{
	struct SingleSnakeGridViewModel
	{
		size_t gridWidth = 3;
		size_t gridHeight = 3;
		GameView::IGameRenderer* gameRenderer;
		const GameOptions* gmOptions;
		GameLogic::CellInterpreter::ICellInterpreterPtr celInterpreter;
	};
	
	class SingleSnakeGridView : public SingleSnakeEnvBase
	{
	public:
		SingleSnakeGridView(const SingleSnakeGridViewModel& model);
		
		size_t GetNumbOfObservations() const override;
		std::vector<int> GetFieldOfView() const;
	
	private:
		const size_t m_gridWidth;
		const size_t m_gridHeight;
	};
}
