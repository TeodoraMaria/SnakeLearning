#pragma once


#include "AI/ITrainer.hpp"
#include "GymEnv/SingleSnakeEnvBase.hpp"

namespace AI{namespace GeneticAlgorithm
{
    class GeneticTrainer :public AI::ITrainer
    {
    public:
        GeneticTrainer(GymEnv::SingleSnakeEnvBase* env);
        virtual IPlayer * Train() override;

    private:
        const size_t numEpisodes = 50000;
        const size_t maxNumSteps = 100;

        //void runEpisode();

        //void runStep();

    };


}}