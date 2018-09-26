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
        GeneticTrainer(Utils::NetworkSettings networkSettings, GeneticOptions options, GymEnv::SingleSnakeEnvBase* env);
        virtual IPlayer * Train(TrainCallbacks callbacks) override;

    private:
        void runEpisode(size_t episode);
        double runStep(const std::vector<double>& state, const GeneticNetwork& network);
        void crossover();
        void selectNewNetworks();
        void mutate();
        void printFitnessInfo();
        void resetFitness();
        


        GymEnv::SingleSnakeEnvBase* m_env;
        GeneticOptions m_options;
        std::vector<GeneticNetwork> m_networks;
    };
}}