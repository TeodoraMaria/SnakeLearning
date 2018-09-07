#pragma once

#include "GameLogic/GameUtils.h"
#include <vector>

namespace ActionPicking
{
	class IActionPicker
	{
	public:
		virtual SnakeMove PickMove(
			const std::vector<double>& observations,
			double noise = 0) = 0;
	};
}
