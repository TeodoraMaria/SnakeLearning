#pragma once

#include "GameLogic/GameState.h"
#include "GameLogic/CellInterpreter/ICellInterpreter.hpp"
#include <functional>

namespace GymEnv { namespace Utils
{
	class StateExtractor
	{
	public:
		static int BinaryVectorToNumber(
			const std::vector<double>& binTable,
			size_t cellStates);
	};
}}
