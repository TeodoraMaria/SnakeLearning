#include "GeneticBot.h"
#include "GeneticNetwork.h"

using namespace AI::GeneticAlgorithm;


GeneticBot::GeneticBot(const GeneticNetwork & network):m_network(m_network)
{

}

SnakeMove GeneticBot::GetNextAction(const GameState & gameState) const
{
    const GameBoard& board = gameState.GetGameBoard();

    
    

    return SnakeMove::FORWARD;
}

