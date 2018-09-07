#pragma once

#include "GameLogic/IPlayer.h"
#include "GeneticNetwork.h"

namespace AI{ namespace GeneticAlgorithm
{
    class GeneticBot : public IPlayer
    {
    public:

        GeneticBot();
        SnakeMove GetNextAction(const GameState& gameState) override;

        void setNetwork(const GeneticNetwork& network);

    private:
        GeneticNetwork m_network;
    };
}}
