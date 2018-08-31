#include "SingleSnakeRelativeView.hpp"
#include "SnakeStudent.hpp"
#include <cmath>
#include <assert.h>

using namespace GymEnv;

const std::vector<SnakeMove> SingleSnakeRelativeView::actions =
{{
	SnakeMove::LEFT,
	SnakeMove::FORWARD,
	SnakeMove::RIGHT,
}};

SingleSnakeRelativeView::SingleSnakeRelativeView()
{
	m_student = std::make_shared<SnakeStudent>();
}

size_t SingleSnakeRelativeView::GetNumbOfObservations() const
{
	static const size_t viewGridSize = 3;
	
	// 0 - Nothing, 1 - Wall, 2 - Food.
	static const size_t cellStatesCount = 3;
	
	return std::pow(viewGridSize, cellStatesCount);
}

void SingleSnakeRelativeView::Reset()
{
	auto players = std::vector<IPlayerPtr>(
	{
		IPlayerPtr(m_student)
	});
	
	const auto gameOptions = GameOptions(
		GameBoardType::BOX,
		10,
		20,
		players.size());
	
	m_game = std::make_unique<Game>(Game(gameOptions, players));
	m_game->InitGame();
}

int SingleSnakeRelativeView::GetState() const
{
	return m_stateExtractor.GetRelativeViewStateBase3(
		m_game->GetGameState(),
		m_student->GetSnakeNumber());
}

StepResult SingleSnakeRelativeView::Step(const SnakeMove moveDirection)
{
	m_student->SetNextAction(moveDirection);
	
	auto stepResult = StepResult();
	stepResult.reward = m_game->MoveSnake(
		m_student->GetSnakeNumber(),
		moveDirection);
	
	stepResult.isDone = stepResult.reward == -1;
	return stepResult;
}

void SingleSnakeRelativeView::Render()
{
	m_game->PrintBoard();
}
