#pragma once

namespace AI{namespace GeneticAlgorithm
{
    
    struct GeneticOptions
    {
        size_t numEpisodes;
        size_t maxNumSteps;
        size_t numOfNetworks;
        double mutationProb;
        double crossoverProb;
    };
    
}}
