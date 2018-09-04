#pragma once

#include "SingleSnakeEnvBase.hpp"
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
	class SingleSnakeRelativeView : public SingleSnakeEnvBase
	{
	public:
		SingleSnakeRelativeView(
			GameView::IGameRenderer* gameRenderer,
			const GameOptions& gmOptions);
		
		size_t GetNumbOfObservations() const override;
		const std::vector<int> GetState() const override;
	};
}
