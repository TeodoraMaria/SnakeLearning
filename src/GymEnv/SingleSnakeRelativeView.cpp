#include "SingleSnakeRelativeView.hpp"
#include "SnakeStudent.hpp"
#include <cmath>
#include <assert.h>

using namespace GymEnv;

const std::array<int, 3> SingleSnakeRelativeView::actions = {{-1, 0, 1}};

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

/*
**	for cell, cell_index in viewGrid:
**		if cell != 0:
**			state += cell * 3 ^ cell_index
**
**	Where cell is { 0, NonFood = 1, FOOD = 2 }
*/

int SingleSnakeRelativeView::GetState() const
{
	const auto gameState = m_game->GetGameState();
	
	const auto& snake = *std::find_if(
		gameState.GetSnakes().begin(),
		gameState.GetSnakes().end(),
		[&](const auto& snake)
		{
			return snake.GetSnakeNumber() == m_student->GetSnakeNumber();
		});

	const auto& head = snake.GetSnakeHead();
	const auto snakeOrientation = snake.GetOrientation();
	
	const auto left = head + snakeOrientation.Rotate90Left();
	const auto right = head + snakeOrientation.Rotate90Right();
	const auto forward = head + snakeOrientation;
	
	const auto viewGrid = std::array<Coordinate, 3>({{left, forward, right}});

	int state = 0;
	for (auto i = 0u; i < viewGrid.size(); i++)
	{
		if (viewGrid[i] == 0)
			continue;
		
		auto cellValue = 0;
		
		// Non food.
		if (!(viewGrid[i] == 1))
			cellValue = 1;
		
		// Food
		if (viewGrid[i] == 1)
			cellValue = 2;
		
		state += cellValue * std::pow(3, i);
	}
	
	assert(state < GetNumbOfObservations());
 	return state;
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
