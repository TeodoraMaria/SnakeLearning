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
	class SingleSnakeGridView : public SingleSnakeEnvBase
	{
	public:
		SingleSnakeGridView(
			size_t gridWidth,
			size_t gridHeight,
			GameView::IGameRenderer* gameRenderer,
			const GameOptions& gmOptions);
		
		size_t GetNumbOfObservations() const override;
		int GetState() const override;
		FieldOfView GetFieldOfView() const;
	
	private:
		const size_t m_gridWidth;
		const size_t m_gridHeight;
	};
}
