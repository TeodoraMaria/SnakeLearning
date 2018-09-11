#include "GeneticBot.h"
#include "GeneticNetwork.h"

using namespace AI::GeneticAlgorithm;


GeneticBot::GeneticBot(const GeneticNetwork& network, std::shared_ptr<GymEnv::StateObserver::IStateObserver> observer) :
    m_network(network),
    m_observer(observer)
{
    
}

SnakeMove GeneticBot::GetNextAction(const GameState & gameState)
{
    const GameBoard& board = gameState.GetGameBoard();

    std::vector<double> input;

    m_observer->Observe(input,gameState,GetSnakeNumber());

    return m_network.feedForward(input);
}

