#include "SingleSnakeGridView.hpp"
#include "Utils/PrintUtils.h"
#include <cmath>

using namespace GymEnv;
using namespace GymEnv::StateObserver;

SingleSnakeGridView::SingleSnakeGridView(
	const SingleSnakeGridViewModel& model
) :
	GymEnv::SingleSnakeEnvBase(model.baseModel)
{
	m_gridObserver = std::make_unique<StateObserver::GridObserver>(
		model.baseModel.celInterpreter,
		model.gridWidth,
		model.gridHeight,
		model.deltaCoord);
}

const IStateObserver* SingleSnakeGridView::GetObserver() const
{
	return m_gridObserver.get();
}

size_t SingleSnakeGridView::GetNumbOfObservations() const
{
	return std::pow(
		m_celInterpreter->NbOfInterpretableParts(),
		m_gridObserver->GetWidth() * m_gridObserver->GetHeight());
}

std::vector<double> SingleSnakeGridView::GetState() const
{
	const auto gmState = m_game->GetGameState();
	const auto snake = gmState.GetSnake(m_student->GetSnakeNumber());
	
	static auto observationContainer =
		std::vector<double>(m_gridObserver->NbOfObservations());
	
	m_gridObserver->Observe(
		observationContainer,
		gmState,
		m_student->GetSnakeNumber());
	
	return observationContainer;
}
