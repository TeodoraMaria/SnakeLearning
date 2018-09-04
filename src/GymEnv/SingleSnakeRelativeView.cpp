#include "SingleSnakeRelativeView.hpp"
#include "SnakeStudent.hpp"
#include <cmath>
#include <assert.h>

using namespace GymEnv;

SingleSnakeRelativeView::SingleSnakeRelativeView(
	GameView::IGameRenderer* gameRenderer,
	const GameOptions& gmOptions) :
	SingleSnakeEnvBase(gameRenderer, gmOptions)
{
}

size_t SingleSnakeRelativeView::GetNumbOfObservations() const
{
	// 0 - Nothing, 1 - Wall, 2 - Food.
	static const size_t cellStatesCount = 3;
	static const size_t viewGridSize = 3;
	
	return std::pow(viewGridSize, cellStatesCount);
}

int SingleSnakeRelativeView::GetState() const
{
	return GymEnv::Utils::StateExtractor::GetRelativeViewStateBase3(
		m_game->GetGameState(),
		m_student->GetSnakeNumber());
}
