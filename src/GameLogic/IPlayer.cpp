#include "IPlayer.h"

int IPlayer::GetSnakeNumber() const
{
	return m_snakeNumber;
}

void IPlayer::SetSnakeNumber(const int snakeNumber)
{
	m_snakeNumber = snakeNumber;
}

bool IPlayer::GetIsActive() const
{
	return m_isActive;
}

void IPlayer::SetIsActive(bool isActive)
{
	m_isActive = isActive;
}
