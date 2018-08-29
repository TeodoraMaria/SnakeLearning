#include "Bot.hpp"

using namespace AI::HardCoded;

size_t Bot::GetSnakeNumber() const
{
	return m_snakeNb;
}

void Bot::SetSnakeNumber(const size_t& snakeNumber)
{
	m_snakeNb = snakeNumber;
}

SnakeMove Bot::GetNextAction() const
{
	throw;
}
