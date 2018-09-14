#pragma once

#include "GameLogic/IPlayer.h"
#include "GeneticNetwork.h"
#include "GymEnv/StateObserver/GridObserver.hpp"

#include <json_fwd.hpp>

namespace AI{ namespace GeneticAlgorithm
{
    class GeneticBot : public IPlayer
    {
    public:

        GeneticBot(const GeneticNetwork& network, std::shared_ptr<GymEnv::StateObserver::IStateObserver> observer);
        SnakeMove GetNextAction(const GameState& gameState) override;

        friend void to_json(nlohmann::json& j, const GeneticBot* player);

    private:
        GeneticNetwork m_network;

        std::shared_ptr<GymEnv::StateObserver::IStateObserver> m_observer;

    };
}}
