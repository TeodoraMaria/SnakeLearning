#pragma once

#include "QTable.h"
#include "GameLogic/IPlayer.h"
#include "GymEnv/StateExtractor.hpp"

namespace AI { namespace QLearning { namespace TrainedAgent
{
	class TrainedTabularAgent : public IPlayer
	{
	public:
		TrainedTabularAgent(
			const std::vector<SnakeMove>& actions,
			const QTable& qBrains);
		
		size_t GetSnakeNumber() const override;
		void SetSnakeNumber(const size_t& snakeNumber) override;
		
		SnakeMove GetNextAction(const GameState& gameState) const override;
	
	private:
		const GymEnv::Utils::StateExtractor m_stateExtractor;
		const std::vector<SnakeMove> m_actions;
		QTable m_qBrains;
		size_t m_snakeNb;
	};
}}}
