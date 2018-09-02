#pragma once

#include "AI/ITrainer.hpp"
#include "GymEnv/SingleSnakeRelativeView.hpp"
#include "QTable.h"
#include <cstddef>
#include <random>
#include <map>

namespace AI{ namespace QLearning
{
	class TabularTrainer : public AI::ITrainer
	{
	public:
		TabularTrainer();
		IPlayer* Train() override;
	
	private:
		typedef int State;
		
		struct TrainSession;
		struct TrainStepResult;
	
		const double learningRate = 0.1;
		const double qDiscountFactor = 0.99;
		const double numEpisodes = 3000;
		const double maxNumSteps = 100;
		
		const double maxRandActionChance = 0.9;
		const double minRandActionChance = 0.00;
		const double randActionDecayFactor = 1.0 / 8000;
		
		// Reward stuff.
		const double foodReward = 1;
		const double dieReward = -1;
		const double stepReward = -0.005;
		
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
		GymEnv::SingleSnakeRelativeView m_env;
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
