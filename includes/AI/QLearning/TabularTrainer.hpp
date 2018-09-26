#pragma once

#include "QTable.h"
#include "QOptions.h"
#include "AI/ITrainer.hpp"
#include "GymEnv/SingleSnakeEnvBase.hpp"
#include <cstddef>
#include <random>
#include <unordered_map>

namespace AI { namespace QLearning
{
	class TabularTrainer : public AI::ITrainer
	{
	public:
		TabularTrainer(QOptions& qoptions, GymEnv::SingleSnakeEnvBase* env);
		
		const QTable& GetQTable() const;
		void SetQTable(const QTable& qtable);
		
		IPlayerPtr Train(TrainCallbacks callbacks) override;
	
	private:
		typedef unsigned long long State;
		
		struct TrainSession;
		struct TrainStepResult;
		
		bool IsRenderPhase(const int episode) const;
		
		void RunEpisode(TrainSession& trainSession);
		TrainStepResult RunStep(
			State currentState,
			const TrainSession& trainSession);

		double ComputeStepReward(
			const GymEnv::StepResult& stepResult,
			const int episode) const;
			
		double UpdateActionQuality(
			State currentState,
			State newState,
			int actionIndex,
			double actionReward,
			bool isDone);
		
		void TryInitQField(State key);
		
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
