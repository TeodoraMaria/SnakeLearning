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

int HumanPlayer::GetNextAction() const
{
	std::cout << "Action for snake " << m_snakeNumber << std::endl;
	int action = getch();
	action = getch();
	switch (action) {
	case KEY_UP:
		return 0;
	case KEY_LEFT:
		return -1;
	case KEY_RIGHT:
		return 1;
	default:
		return 0;
	}
}
