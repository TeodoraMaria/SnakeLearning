#pragma once

#include "GameLogic/IPlayer.h"
#include "GeneticNetwork.h"
#include "GymEnv/StateObserver/GridObserver.hpp"

namespace AI{ namespace GeneticAlgorithm
{
    class GeneticBot : public IPlayer
    {
    public:

        GeneticBot(const GeneticNetwork& network, std::shared_ptr<GymEnv::StateObserver::IStateObserver> observer);
        SnakeMove GetNextAction(const GameState& gameState) const override;

    private:
        GeneticNetwork m_network;

        std::shared_ptr<GymEnv::StateObserver::IStateObserver> m_observer;

    };
}}