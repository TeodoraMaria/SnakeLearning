#pragma once

#include "AI/ITrainer.hpp"
#include "GymEnv/SingleSnakeEnvBase.hpp"
#include "GeneticOptions.h"
#include "GeneticNetwork.h"
#include "GeneticBot.h"
#include <vector>

namespace AI{namespace GeneticAlgorithm
{
    class GeneticTrainer :public AI::ITrainer
    {
    public:
        GeneticTrainer(GeneticOptions options,GymEnv::SingleSnakeEnvBase* env);
        virtual IPlayer * Train() override;

    private:
        void runEpisode();
        double runStep(const std::vector<int>& state, const GeneticNetwork& network);
        void runBot(GeneticNetwork& network);
        void crossover();
        void selectNewNetworks();
        void mutate();
        


        GymEnv::SingleSnakeEnvBase* m_env;
        GeneticOptions m_options;
        std::vector<GeneticNetwork> m_networks;
        std::vector<double> m_networkProb;


    };
}}