#include "IPlayer.h"

std::vector<SnakeMove> IPlayer::possibleMoves =
{
	SnakeMove::LEFT,
	SnakeMove::FORWARD,
	SnakeMove::RIGHT
};

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
