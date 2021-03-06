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
		MultisnakeTabularTrainer(bool renderByYourself = false);
	
		IPlayerPtr Train(TrainCallbacks callbacks) override;
		
	private:
		void TryInitQField(QTable& qtable, const State key);
	
		GameOptions m_gmoptions;
		QOptions m_qoptions;
		std::mt19937 m_merseneTwister;
	};
}}
