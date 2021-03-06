#include "HumanPlayer.h"
#include "MultiplatformUtils/Getch.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

HumanPlayer::HumanPlayer()
{
}

HumanPlayer::~HumanPlayer()
{
}

#define KEY_UP		'w'
#define KEY_LEFT	'a'
#define KEY_RIGHT	'd'
#define KEY_DOWN	's'

SnakeMove HumanPlayer::GetNextAction(const GameState& gameState)
{
	SnakeMove move = GetUserInput();

	TranslateMoveToSnakeView(move, gameState);
	return move != SnakeMove::DOWN ? move:SnakeMove::FORWARD;
}

SnakeMove HumanPlayer::GetUserInput() const
{
	std::cout << "Action for snake " << GetSnakeNumber() << std::endl;
	const int action = MultiplatformUtils::Getch();
	
	switch (action)
	{
	case KEY_UP:
		return SnakeMove::FORWARD;
	case KEY_LEFT:
		return SnakeMove::LEFT;
	case KEY_RIGHT:
		return SnakeMove::RIGHT;
	case KEY_DOWN:
		return SnakeMove::DOWN;
	default:
		std::cout << "Invalid input";
		return SnakeMove::FORWARD;
	}
}

void HumanPlayer::TranslateMoveToSnakeView(SnakeMove & move, const GameState& gameState) const
{
	const auto& snake = gameState.GetSnake(GetSnakeNumber());
	auto orientation = snake.GetOrientation();
	if (orientation == Coordinate::RIGHT)
	{
		RotateLeft(move);
	}
	else if (orientation == Coordinate::DOWN)
	{
		RotateLeft(move);
		RotateLeft(move);
	}
	else if (orientation == Coordinate::LEFT)
	{
		RotateLeft(move);
		RotateLeft(move);
		RotateLeft(move);
	}
}

void HumanPlayer::RotateLeft(SnakeMove & move) const
{
	switch (move)
	{
		case SnakeMove::FORWARD:
			move = SnakeMove::LEFT;
			return;
		case SnakeMove::LEFT:
			move = SnakeMove::DOWN;
			return;
		case SnakeMove::DOWN:
			move = SnakeMove::RIGHT;
			return;
		case SnakeMove::RIGHT:
			move = SnakeMove::FORWARD;
			return;
	}
}
