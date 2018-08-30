#include "HumanPlayer.h"
#include "MultiPlatformTerm.hpp"
#include <iostream>

HumanPlayer::HumanPlayer()
{
}

HumanPlayer::~HumanPlayer()
{
}

size_t HumanPlayer::GetSnakeNumber() const
{
	return m_snakeNumber;
}

void HumanPlayer::SetSnakeNumber(const size_t & snakeNumber)
{
	m_snakeNumber = snakeNumber;
}


#define KEY_UP 'w'
#define KEY_LEFT 'a'
#define KEY_RIGHT 'd'

SnakeMove HumanPlayer::GetNextAction(const GameState& gameState) const
{
	std::cout << "Action for snake " << m_snakeNumber << std::endl;
	int action = MultiPlatform::getch();
	switch (action) {
	case KEY_UP:
		return SnakeMove::FORWARD;
	case KEY_LEFT:
		return SnakeMove::LEFT;
	case KEY_RIGHT:
		return SnakeMove::RIGHT;
	default:
		return SnakeMove::FORWARD;
	}
}
