#include "SingleSnakeGridView.hpp"
#include <cmath>

using namespace GymEnv;

SingleSnakeGridView::SingleSnakeGridView(
	const size_t gridWidth,
	const size_t gridHeight,
	GameView::IGameRenderer* const gameRenderer,
	const GameOptions& gmOptions
) :
	GymEnv::SingleSnakeEnvBase(gameRenderer, gmOptions),
	m_gridWidth(gridWidth),
	m_gridHeight(gridHeight)
{
}

size_t SingleSnakeGridView::GetNumbOfObservations() const
{
	// 3 = Empty | Food | Wall / Body
	return std::pow(3, m_gridWidth * m_gridHeight);
}

int SingleSnakeGridView::GetState() const
{
	const auto gameState = m_game->GetGameState();
	const auto fieldOfView = gameState.GetFieldOfView(
		gameState.GetSnake(m_student->GetSnakeNumber()),
		m_gridWidth,
		m_gridHeight);
	
	assert(fieldOfView.size() == m_gridHeight);
	assert(fieldOfView.front().size() == m_gridWidth);
	
	return GymEnv::Utils::StateExtractor::GetGridViewState(
		gameState.GetGameBoard(),
		fieldOfView);
}
