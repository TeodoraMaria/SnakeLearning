#pragma once

#include "GameLogic/IPlayer.h"
#include "GeneticNetwork.h"

namespace AI{ namespace GeneticAlgorithm
{
    class GeneticBot : public IPlayer
    {
    public:

        GeneticBot(const GeneticNetwork& network);
        SnakeMove GetNextAction(const GameState& gameState) const override;

    private:
        GeneticNetwork m_network;
    };
}}