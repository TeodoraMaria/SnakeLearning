#pragma once

#include "AI/ITrainer.hpp"
#include "GymEnv/StepResult.h"
#include "QTable.h"
#include <cstddef>
#include <random>

namespace AI{ namespace QLearning
{
	class TabularTrainer : public AI::ITrainer
	{
	public:
		TabularTrainer();
		IPlayer* Train() override;
	
	private:
		const double learningRate = 0.1;
		const double qDiscountFactor = 0.99;
		const double numEpisodes = 1000;
		const double maxNumSteps = 10000;
		
		const double maxRandActionChance = 0.9;
		const double minRandActionChance = 0.00;
		const double randActionDecayFactor = 1.0 / 8000;
		
		// Reward stuff.
		const double foodReward = 1;
		const double dieReward = -1;
		const double stepReward = -0.005;
		
		double ComputeStepReward(const GymEnv::StepResult& stepResult) const;
		double UpdateActionQuality(
			int currentState,
			int newState,
			int actionIndex,
			double actionReward,
			bool isDone);
		
		std::mt19937 m_merseneTwister;
		QTable m_qtable;
	};
}}
