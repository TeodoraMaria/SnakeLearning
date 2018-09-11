#pragma once

#include "QTable.h"
#include "State.h"

#include "GameLogic/CellInterpreter/ICellInterpreter.hpp"
#include "GameLogic/GameState.h"

#include "GymEnv/StateObserver/IStateObserver.hpp"
#include "GymEnv/SnakeStudent.hpp"

#include <random>

namespace AI { namespace QLearning
{
	class QTabStudent : public IPlayer
	{
	public:
		QTabStudent(
			std::shared_ptr<GameLogic::CellInterpreter::ICellInterpreter> cellInterpreter,
			std::shared_ptr<GymEnv::StateObserver::IStateObserver> observer,
			std::function<double ()> qtabInitializer = []() { return 0.0; },
			double actionQulityCompareEps = 0.0);
	
		double GetReward() const;
		void SetReward(double newValue);
		
		size_t GetStepsWithoutFood() const;
		void SetStepsWithoutFood(size_t newVal);
		
		double GetNoise() const;
		void SetNoise(double newValue);
		
		const QTable& GetQTab() const;
		void SetQTab(const QTable& newQTab);
		
		const GymEnv::StateObserver::IStateObserver* GetObserver() const;
		
		SnakeMove GetNextAction(const GameState& gameState) override;
		
		void PrepareForNewEpisode();
		State ObserveState(const GameState& gmState);
		unsigned int PickAction(State fromState);
		double GetBestQualityFromState(State state);
		
		void UpdateQTab(
			State startingState,
			double bestNextQ,
			unsigned int actionIndx,
			double reward,
			double learningRate,
			double qDiscountFactor);
		
		double m_totalReward = 0;
		
	private:
		void TryInitQField(State state);
	
		std::shared_ptr<GameLogic::CellInterpreter::ICellInterpreter> m_cellIntepreter;
		std::shared_ptr<GymEnv::StateObserver::IStateObserver> m_observer;
		
		AI::QLearning::QTable m_qtable;
		std::mt19937 m_merseneTwister;
		double m_actionQulityCompareEps;
		double m_reward = 0;
		size_t m_stepsWithoutFood = 0;
		
		std::vector<double> m_observationContainer;
		double m_noise = 0;
		std::function<double ()> m_qtabInitializer;
	};
}}
