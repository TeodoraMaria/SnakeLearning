#pragma once

#include "SnakeEnvBase.hpp"

namespace GymEnv
{
	struct MultiSnakeEnvModel
	{
		GameView::IGameRenderer* gameRenderer = nullptr;
		size_t nbOfSnakes = 1;
	};

	class MultiSnakeEnv : public SnakeEnvBase
	{
	
	};
}
