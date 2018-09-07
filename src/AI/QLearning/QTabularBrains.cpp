#include "QTabularBrains.hpp"
#include "Utils/MatrixUtils.h"

using namespace AI::QLearning;

QTabularBrains::QTabularBrains(
	double learningRate,
	double qDiscountFactor,
	size_t nbOfActions,
	std::function<double ()> qInitializer
) :
	m_learningRate(learningRate),
	m_qDiscountFactor(qDiscountFactor),
	m_qtable(),
	m_nbOfActions(nbOfActions),
	m_qInitializer(qInitializer)
{
}

const std::vector<double>& QTabularBrains::GetQTabRow(const State state) const
{
	TryInitQField(state);
	return m_qtable[state];
}

void QTabularBrains::UpdateActionQuality(
	const State initialState,
	const State finalState,
	const double reward,
	const size_t actionIndex,
	const bool isDone)
{
	double bestNextQ;
	
	if (isDone)
		bestNextQ = 0;
	else
	{
		TryInitQField(finalState);
		bestNextQ = *std::max_element(
			m_qtable[finalState].begin(),
			m_qtable[finalState].end());
	}

	TryInitQField(initialState);
	
	const auto currentActionQ = m_qtable[initialState][actionIndex];
	m_qtable[initialState][actionIndex] +=
		m_learningRate *
		(reward + m_qDiscountFactor * bestNextQ - currentActionQ);
	
}

/*
** Private helpers.
*/

void QTabularBrains::TryInitQField(const State state) const
{
	if (m_qtable.find(state) == m_qtable.end())
	{
		m_qtable[state] = ::Utils::Matrix::MakeVector(
			m_nbOfActions,
			m_qInitializer);
	}
}
