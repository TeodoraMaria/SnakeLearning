#include "GeneticBot.h"
#include "GeneticNetwork.h"

using namespace AI::GeneticAlgorithm;

GeneticBot::GeneticBot()
{}

SnakeMove GeneticBot::GetNextAction(const GameState & gameState) const
{
    const GameBoard& board = gameState.GetGameBoard();

    return SnakeMove::FORWARD;
}

void GeneticBot::setNetwork(const GeneticNetwork & network)
{
    m_network = network;
}
