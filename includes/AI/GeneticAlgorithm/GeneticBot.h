#pragma once

#include "GameLogic/IPlayer.h"

namespace AI{ namespace GeneticAlgorithm
    {
        class GeneticBot : public IPlayer
        {
        public:

            GeneticBot();
            SnakeMove GetNextAction(const GameState& gameState) const override;
        };
    }
}