#include <iostream>
#include <conio.h>
#include "HumanPlayer.h"

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

#define KEY_UP 72
#define KEY_LEFT 75
#define KEY_RIGHT 77

SnakeMove HumanPlayer::GetNextAction() const
{
	std::cout << "Action for snake " << m_snakeNumber << std::endl;
	int action = getch();
	action = getch();
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