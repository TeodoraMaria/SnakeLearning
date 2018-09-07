#pragma once

#include "IActionPicker.hpp"
#include "StateObserver/IStateObserver.hpp"
#include "GameLogic/IPlayer.h"
#include "GameLogic/CellInterpreter/ICellInterpreter.hpp"

namespace GymEnv
{
	class SnakeAgent : public IPlayer
	{
	public:
		SnakeAgent(
			GameLogic::CellInterpreter::ICellInterpreterPtr cellInterpreter,
			std::shared_ptr<ActionPicking::IActionPicker> actionPicker,
			std::shared_ptr<StateObserver::IStateObserver> observer);
	
		const GameLogic::CellInterpreter::ICellInterpreter& GetCellInterpreter() const;
		const StateObserver::IStateObserver& GetObserver() const;
	
		SnakeMove GetNextAction(const GameState& gameState) override;
		SnakeMove GetNextAction(
			const std::vector<double>& observations,
			double noise = 0.0);
		
	private:
		GameLogic::CellInterpreter::ICellInterpreterPtr m_cellInterpreter;
		std::shared_ptr<ActionPicking::IActionPicker> m_actionPicker;
		std::shared_ptr<StateObserver::IStateObserver> m_observer;
		
		std::vector<double> m_observationContainer;
	};
}
