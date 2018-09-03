#pragma once

#include "GameLogic/GameState.h"

namespace GymEnv { namespace Utils
{
	class StateExtractor
	{
	public:
		static int GetRelativeViewStateBase3(
			const GameState& gmState,
			int snakeId);
		
	private:
		static const int emptySpaceValue = 0;
		static const int wallValue = 1;
		static const int foodValue = 2;
		
		static int ComputeVieGridValue(
			const GameBoard& gmBoard,
			const std::vector<Coordinate>& viewGrid,
			int base = 3);
	};
}}
