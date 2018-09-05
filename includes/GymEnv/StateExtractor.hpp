#pragma once

#include "GameLogic/GameState.h"
#include "GameLogic/CellInterpreter/ICellInterpreter.hpp"
#include <functional>

namespace GymEnv { namespace Utils
{
	class StateExtractor
	{
	public:
		static void GetRelativeViewState(
			std::vector<int>& dest,
			const Snake& snake,
			const GameBoard& gmBoard);

		static int GetRelativeViewStateBase3(
			const GameState& gmState,
			int snakeId);
		
		static int BinaryVectorToNumber(
			const std::vector<double>& binTable,
			size_t cellStates);
		
		static int GetGridViewState(
			const GameBoard& gmBoard,
			const std::vector<int>& fieldOfVIew,
			size_t numCellStates = 3);
		
	private:
		static const int emptySpaceValue = 0;
		static const int wallValue = 1;
		static const int foodValue = 2;
		
		static int ComputeVieGridValue(
			size_t numCells,
			std::function<int (int)> getValue,
			int base = 3);
		
		static int GetGameCellValue(int gameCellValue);
	};
}}
