#include "SingleSnakeRelativeView.hpp"
#include "SnakeStudent.hpp"
#include "GymEnv/StateObserver/ThreeDirectionalObserver.hpp"
#include <cmath>
#include <assert.h>

using namespace GymEnv;
using namespace GymEnv::StateObserver;

SingleSnakeRelativeView::SingleSnakeRelativeView(
	const SingleSnakeEnvBaseModel& baseModel) :
	SingleSnakeEnvBase(baseModel)
{
	m_stateObserver =
		std::make_unique<ThreeDirectionalObserver>(baseModel.celInterpreter);
}

const IStateObserver* SingleSnakeRelativeView::GetObserver() const
{
	return m_stateObserver.get();
}

size_t SingleSnakeRelativeView::GetNumbOfObservations() const
{
	static const size_t viewGridSize = 3;
	
	return std::pow(viewGridSize, m_celInterpreter->NbOfInterpretableCells());
}

std::vector<double> SingleSnakeRelativeView::GetState() const
{
	const auto gmState = m_game->GetGameState();
	const auto snake = gmState.GetSnake(m_student->GetSnakeNumber());
	
	const auto containerLen =
		3 * (m_celInterpreter->NbOfInterpretableCells() - 1);
	
	auto observationContainer = std::vector<double>(containerLen);
	
	m_stateObserver->Observe(
		observationContainer,
		gmState,
		m_student->GetSnakeNumber());
	
	return observationContainer;
}
