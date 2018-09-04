#include "SingleSnakeGridView.hpp"
#include "Utils/PrintUtils.h"
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

//const std::vector<int>& SingleSnakeGridView::GetState() const
//{
//	const auto gameState = m_game->GetGameState();
//	const auto fieldOfView = GetFieldOfView();
//	
//	return GymEnv::Utils::StateExtractor::GetGridViewState(
//		gameState.GetGameBoard(),
//		fieldOfView);
//}

std::vector<int> SingleSnakeGridView::GetFieldOfView() const
{
	const auto gameState = m_game->GetGameState();
	return gameState.GetFieldOfView(
		gameState.GetSnake(m_student->GetSnakeNumber()),
		m_gridHeight,
		m_gridWidth);
}
