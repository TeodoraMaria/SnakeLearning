#include "SingleSnakeEnvBase.hpp"

using namespace GymEnv;

SingleSnakeEnvBase::SingleSnakeEnvBase(
	GameView::IGameRenderer* const gmRenderer,
	const GameOptions& gmOptions) :
	SnakeEnvBase(gmRenderer),
	m_student(std::make_shared<SnakeStudent>())
{
	auto players = std::vector<IPlayerPtr>(
	{
		IPlayerPtr(m_student)
	});
	
	m_game = std::make_unique<Game>(Game(gmOptions, players));
}

void SingleSnakeEnvBase::Reset()
{
	m_game->InitGame();
}

StepResult SingleSnakeEnvBase::Step(const SnakeMove moveDirection)
{
	m_student->SetNextAction(moveDirection);
	
	auto stepResult = StepResult();
	stepResult.reward = m_game->MoveSnake(
		m_student->GetSnakeNumber(),
		moveDirection);
	m_game->RestockFood();
	
	stepResult.isDone = (stepResult.reward == -1);
	return stepResult;
}
