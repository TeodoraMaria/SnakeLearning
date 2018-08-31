#include "SnakeStudent.hpp"

using namespace GymEnv;

/*
** Getters & setters.
*/

SnakeMove SnakeStudent::GetNextAction(const GameState& gameState) const
{
	return m_nextMove;
}

void SnakeStudent::SetNextAction(const SnakeMove nextMove)
{
	m_nextMove = nextMove;
}
