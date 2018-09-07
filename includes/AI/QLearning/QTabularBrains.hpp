#pragma once

#include "QTable.h"
#include "State.h"
#include <functional>

namespace AI { namespace QLearning
{
	class QTabularBrains
	{
	public:
		QTabularBrains(
			double learningRate,
			double qDiscountFactor,
			size_t nbOfActions,
			std::function<double ()> qInitializer);
	
		const std::vector<double>& GetQTabRow(State state) const;
	
		void UpdateActionQuality(
			State initialState,
			State finalState,
			double reward,
			size_t actionIndex,
			bool isDone);
		
	private:
		void TryInitQField(const State state) const;
	
		const double m_learningRate;
		const double m_qDiscountFactor;
	
		mutable QTable m_qtable;
		const size_t m_nbOfActions;
		const std::function<double ()> m_qInitializer;
	};
}}
