#pragma once

#include "QTable.h"
#include "QOptions.h"
#include "AI/ITrainer.hpp"
#include "GymEnv/SingleSnakeEnvBase.hpp"
#include <cstddef>
#include <random>
#include <map>

namespace AI{ namespace QLearning
{
	class TabularTrainer : public AI::ITrainer
	{
	public:
		TabularTrainer(GymEnv::SingleSnakeEnvBase* env);
		IPlayer* Train() override;
	
	private:
		typedef int State;
		
		struct TrainSession;
		struct TrainStepResult;
	
		const double learningRate = 0.1;
		const double qDiscountFactor = 0.99;
		const double numEpisodes = 600000;
		const double maxNumSteps = 200;
		
		const double maxRandActionChance = 1;
		const double minRandActionChance = 0.00;
		const double randActionDecayFactor = 1.0 / (numEpisodes * 7);
		
		// Reward stuff.
		const double foodReward = 1;
		const double dieReward = -1;
		const double stepReward = -0.001;
		
		void RunEpisode(TrainSession& trainSession);
		TrainStepResult RunStep(
			State currentState,
			double randomActionChance);

		double ComputeStepReward(const GymEnv::StepResult& stepResult) const;
		double UpdateActionQuality(
			int currentState,
			int newState,
			int actionIndex,
			double actionReward,
			bool isDone);
		
		std::mt19937 m_merseneTwister;
		QTable m_qtable;
		GymEnv::SingleSnakeEnvBase* m_env;
	};
	
	struct TabularTrainer::TrainSession
	{
		int episodeIndex;
		double randomActionChance;
		std::map<State, int> dieStates;
	};
	
	struct TabularTrainer::TrainStepResult
	{
		TabularTrainer::State newState;
		double reward;
		bool isDone = false;
	};
}}
