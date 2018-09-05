#pragma once

#include "QTable.h"
#include "QOptions.h"
#include "AI/ITrainer.hpp"
#include "GymEnv/SingleSnakeEnvBase.hpp"
#include <cstddef>
#include <random>
#include <unordered_map>

namespace AI{ namespace QLearning
{
	class TabularTrainer : public AI::ITrainer
	{
	public:
		TabularTrainer(QOptions& qoptions, GymEnv::SingleSnakeEnvBase* env);
		IPlayer* Train() override;
	
	private:
		typedef int State;
		
		struct TrainSession;
		struct TrainStepResult;
		
		void RunEpisode(TrainSession& trainSession);
		TrainStepResult RunStep(
			State currentState,
			const TrainSession& trainSession);

		double ComputeStepReward(
			const GymEnv::StepResult& stepResult,
			const int episode) const;
			
		double UpdateActionQuality(
			int currentState,
			int newState,
			int actionIndex,
			double actionReward,
			bool isDone);
		
		std::mt19937 m_merseneTwister;
		QTable m_qtable;
		QOptions m_qoptions;
		GymEnv::SingleSnakeEnvBase* m_env;
	};
	
	struct TabularTrainer::TrainSession
	{
		int episodeIndex;
		double randomActionChance;
		std::unordered_map<State, int> dieStates;
	};
	
	struct TabularTrainer::TrainStepResult
	{
		TabularTrainer::State newState;
		double reward;
		bool isDone = false;
	};
}}
