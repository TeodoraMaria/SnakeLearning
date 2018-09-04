#pragma once


#include "AI/ITrainer.hpp"

namespace AI{namespace GeneticAlgorithm
{
    class GeneticTrainer :public AI::ITrainer
    {
        // Inherited via ITrainer
        virtual IPlayer * Train() override;
    };


}}