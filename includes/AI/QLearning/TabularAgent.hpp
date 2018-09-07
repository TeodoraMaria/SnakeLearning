#pragma once

#include "QTabularBrains.hpp"
#include "GymEnv/SnakeAgent.hpp"

namespace AI { namespace QLearning
{
	class TabularAgent : public GymEnv::SnakeAgent
	{
	public:
		TabularAgent(
			std::shared_ptr<QTabularBrains> brains,
			GameLogic::CellInterpreter::ICellInterpreterPtr cellInterpreter,
			std::shared_ptr<ActionPicking::IActionPicker> actionPicker,
			std::shared_ptr<GymEnv::StateObserver::IStateObserver> observer);
		
		QTabularBrains& GetBrains();
		
		State GetState(const GameState& gmState) const;
		State GetState(const std::vector<double>& rawState) const;
		
	private:
		std::shared_ptr<QTabularBrains> m_brains;
	};
}}
