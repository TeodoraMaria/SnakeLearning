#include "SingleSnakeEnvBase.hpp"

using namespace GymEnv;

SingleSnakeEnvBase::SingleSnakeEnvBase(
	GameView::IGameRenderer* const gmRenderer,
	const GameOptions& gmOptions,
	GameLogic::CellInterpreter::ICellInterpreterPtr celInterpreter) :
	SnakeEnvBase(gmRenderer),
	m_student(std::make_shared<SnakeStudent>()),
	m_celInterpreter(celInterpreter)
{
	auto players = std::vector<IPlayerPtr>(
	{
		IPlayerPtr(m_student)
	});
	
	m_game = std::make_unique<Game>(Game(gmOptions, players));
}

GameLogic::CellInterpreter::ICellInterpreterPtr SingleSnakeEnvBase::GetCellInterpreter()
{
	return m_celInterpreter;
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
