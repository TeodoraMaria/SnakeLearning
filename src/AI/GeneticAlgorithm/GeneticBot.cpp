#include "GeneticBot.h"
#include "GeneticNetwork.h"

#include "ConfigLoading/IObserverJson.h"

#include <json.hpp>

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
    input.resize(m_observer->NbOfObservations());

    m_observer->Observe(input,gameState,GetSnakeNumber());

    return m_network.feedForward(input);
}

void AI::GeneticAlgorithm::to_json(nlohmann::json & j, const GeneticBot * player)
{
    j = nlohmann::json{
        {"type", "GeneticBot"},
        {"observer", nlohmann::json(player->m_observer.get())},
        {"input",nlohmann::json(player->m_network.getNetworkSettings().m_inputs)},
        {"network_layers",nlohmann::json(player->m_network.getNetworkSettings().m_hiddenLayersSizes)},
        {"weights", nlohmann::json(player->m_network.getWeights())}
    };
}
