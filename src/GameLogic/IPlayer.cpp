#include "IPlayer.h"

int IPlayer::GetSnakeNumber() const
{
	return m_snakeNumber;
}

void IPlayer::SetSnakeNumber(const int snakeNumber)
{
	m_snakeNumber = snakeNumber;
}
