#pragma once

#include "AI/ITrainer.hpp"
#include "GymEnv/SingleSnakeEnvBase.hpp"
#include "GeneticOptions.h"
#include "GeneticBot.h"
#include <vector>

namespace AI{namespace GeneticAlgorithm
{
    class GeneticTrainer :public AI::ITrainer
    {
    public:
        GeneticTrainer(GymEnv::SingleSnakeEnvBase* env);
        virtual IPlayer * Train() override;

    private:
        void runEpisode();
        void runStep();

        GeneticOptions m_options;
        std::vector<GeneticBot> m_bots;
    };


}}