#include "SnakeStudent.hpp"

using namespace GymEnv;

/*
** Getters & setters.
*/

size_t SnakeStudent::GetSnakeNumber() const
{
	return m_snakeNb;
}

void SnakeStudent::SetSnakeNumber(const size_t& snakeNumber)
{
	m_snakeNb = snakeNumber;
}

SnakeMove SnakeStudent::GetNextAction(const GameState& gameState) const
{
	return m_nextMove;
}

void SnakeStudent::SetNextAction(const SnakeMove nextMove)
{
	m_nextMove = nextMove;
}
