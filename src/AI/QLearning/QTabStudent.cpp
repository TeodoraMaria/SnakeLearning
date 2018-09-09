#include "QTabStudent.hpp"
#include "ActionPickingUtils.h"
#include "GymEnv/StateExtractor.hpp"

#include "Utils/MathUtils.h"
#include "Utils/PrintUtils.h"
#include "Utils/MatrixUtils.h"

using namespace AI::QLearning;
using namespace GameLogic::CellInterpreter;
using namespace GymEnv::StateObserver;
using namespace GymEnv::Utils;

QTabStudent::QTabStudent(
	std::shared_ptr<ICellInterpreter> cellInterpreter,
	IStateObserver* observer,
	std::function<double ()> qtabInitializer
) :
	m_cellIntepreter(cellInterpreter),
	m_observer(observer),
	m_qtabInitializer(qtabInitializer),
	m_observationContainer(m_observer->NbOfObservations())
{
}

/*
** Getters & setters.
*/

double QTabStudent::GetReward() const { return m_reward; }
void QTabStudent::SetReward(double newValue) { m_reward = newValue; }

size_t QTabStudent::GetStepsWithoutFood() const { return m_stepsWithoutFood; }
void QTabStudent::SetStepsWithoutFood(size_t newVal) { m_stepsWithoutFood = newVal; }

double QTabStudent::GetNoise() const { return m_noise; }
void QTabStudent::SetNoise(double newValue) { m_noise = newValue; }

/*
** Public methods.
*/

void QTabStudent::PrepareForNewEpisode()
{
	m_reward = 0;
	m_stepsWithoutFood = 0;
}

State QTabStudent::ObserveState(const GameState& gmState)
{
	m_observer->Observe(
		m_observationContainer,
		gmState,
		GetSnakeNumber());
	
	return StateExtractor::BinaryVectorToNumber(
		m_observationContainer,
		m_cellIntepreter->NbOfInterpretableParts());
}

unsigned int QTabStudent::PickAction(
	State fromState,
	std::mt19937& merseneTwister)
{
	TryInitQField(fromState);
	return QLearning::Utils::PickAction(
		m_qtable[fromState],
		m_noise,
		merseneTwister);
}

double QTabStudent::GetBestQualityFromState(State state)
{
	TryInitQField(state);
	return *std::max_element(
		m_qtable[state].begin(),
		m_qtable[state].end());
}

void QTabStudent::TryInitQField(State state)
{
	if (m_qtable.find(state) == m_qtable.end())
	{
		m_qtable[state] = ::Utils::Matrix::MakeVector(
			IPlayer::possibleMoves.size(),
			m_qtabInitializer);
	}
}

void QTabStudent::UpdateQTab(
	State startingState,
	double bestNextQ,
	unsigned int actionIndx,
	double reward,
	double learningRate,
	double qDiscountFactor)
{
	const auto currentActionQ = m_qtable[startingState][actionIndx];
	m_qtable[startingState][actionIndx] +=
		learningRate *
		(reward + qDiscountFactor * bestNextQ - currentActionQ);
}
