#include "QTabStudent.hpp"
#include "ActionPickingUtils.h"
#include "GymEnv/StateExtractor.hpp"

#include "Utils/MathUtils.h"
#include "Utils/PrintUtils.h"
#include "Utils/MatrixUtils.h"

#include "ConfigLoading/IObserverJson.h"

#include <algorithm>

using namespace AI::QLearning;
using namespace GameLogic::CellInterpreter;
using namespace GymEnv::StateObserver;
using namespace GymEnv::Utils;
using json = nlohmann::json;

QTabStudent::QTabStudent(
	std::shared_ptr<IStateObserver> observer,
	std::function<double ()> qtabInitializer,
	double actionQulityCompareEps
) :
	m_observer(observer),
	m_merseneTwister(std::random_device()()),
	m_actionQulityCompareEps(actionQulityCompareEps),
	m_observationContainer(m_observer->NbOfObservations()),
	m_qtabInitializer(qtabInitializer)
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

const QTable& QTabStudent::GetQTab() const { return m_qtable; }
void QTabStudent::SetQTab(const QTable& newQTab) { m_qtable = newQTab; }

const GymEnv::StateObserver::IStateObserver* QTabStudent::GetObserver() const { return m_observer.get(); }

/*
** JSON helper.
*/

void AI::QLearning::to_json(nlohmann::json& j, const QTabStudent* player)
{
	j = nlohmann::json{
		{ "type", "QTabStudent" },
		{ "noise", player->GetNoise() },
		{ "actionQulityCompareEps", player->m_actionQulityCompareEps },
		{ "observer", json(player->GetObserver()) },
		{ "brains", json(player->GetQTab()) }
	};
}

/*
** Public methods.
*/

SnakeMove QTabStudent::GetNextAction(const GameState& gameState)
{
	return IPlayer::possibleMoves[PickAction(ObserveState(gameState))];
}

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
		m_observer->GetCellInterpreter()->NbOfInterpretableParts());
}

unsigned int QTabStudent::PickAction(State fromState)
{
	TryInitQField(fromState);
	const auto positiveMean = ::Utils::Math::PositiveMean(m_qtable[fromState]);
	return QLearning::Utils::PickActionAdditiveNoise(
		m_qtable[fromState],
		m_noise * positiveMean,
		m_merseneTwister,
		m_actionQulityCompareEps * positiveMean);
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
