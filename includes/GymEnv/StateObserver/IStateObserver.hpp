#pragma once

#include "GameLogic/GameState.h"
#include "GameLogic/CellInterpreter/ICellInterpreter.hpp"
#include <cstddef>
#include <vector>

namespace GymEnv { namespace StateObserver
{
	class IStateObserver
	{
	public:
		IStateObserver(GameLogic::CellInterpreter::ICellInterpreterPtr cellInterpreter);
	
		const GameLogic::CellInterpreter::ICellInterpreter* GetCellInterpreter() const;
	
		virtual size_t NbOfObservations() const = 0;
		virtual void Observe(
			std::vector<double>& observationContainer,
			const GameState& gmState,
			int snakeId) const = 0;
	
	private:
		GameLogic::CellInterpreter::ICellInterpreterPtr m_cellInterpreter;
	};
}}
