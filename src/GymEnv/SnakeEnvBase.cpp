#include "SnakeEnvBase.hpp"

using namespace GymEnv;

const std::vector<SnakeMove> SnakeEnvBase::actions =
{{
	SnakeMove::LEFT,
	SnakeMove::FORWARD,
	SnakeMove::RIGHT,
}};

SnakeEnvBase::SnakeEnvBase(GameView::IGameRenderer* gameRenderer)
{
	if (gameRenderer != nullptr)
		m_gameRenderer = gameRenderer;
}

void SnakeEnvBase::Render() const
{
	if (m_gameRenderer)
		m_gameRenderer->Render(m_game->GetGameState());
}
