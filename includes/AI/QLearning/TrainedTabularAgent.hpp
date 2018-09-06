#pragma once

#include "QTable.h"
#include "GameLogic/IPlayer.h"
#include "GymEnv/StateExtractor.hpp"
#include "GymEnv/StateObserver/IStateObserver.hpp"

namespace AI { namespace QLearning { namespace TrainedAgent
{
	class TrainedTabularAgent : public IPlayer
	{
	public:
		TrainedTabularAgent(
			const std::vector<SnakeMove>& actions,
			size_t interpretableBoardParts,
			const QTable& qBrains,
			std::shared_ptr<GymEnv::StateObserver::IStateObserver> observer);
		
		SnakeMove GetNextAction(const GameState& gameState) const override;
	
	private:
		const std::vector<SnakeMove> m_actions;
		const size_t m_interpretableBoardParts;
		QTable m_qBrains;
		std::shared_ptr<GymEnv::StateObserver::IStateObserver> m_observer;
	};
}}}
