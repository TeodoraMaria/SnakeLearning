#pragma once

#include "QOptions.h"
#include "AI/ITrainer.hpp"
#include "GameLogic/GameOptions.h"
#include "QTable.h"
#include "State.h"
#include <random>

namespace AI { namespace QLearning
{
	class MultisnakeTabularTrainer : public AI::ITrainer
	{
	public:
		MultisnakeTabularTrainer(
			const GameOptions& gmOptions,
			const QOptions& qoptions);
	
		IPlayer* Train() override;
		
	private:
		void TryInitQField(QTable& qtable, const State key);
	
		const GameOptions& m_gmoptions;
		const QOptions& m_qoptions;
		std::mt19937 m_merseneTwister;
	};
}}