#pragma once

#include <GameLogic/GameBoard.h>
#include "Utils/NeuralNetwork.h"

#include <vector>
#include <random>


namespace AI{namespace GeneticAlgorithm{
   
    class GeneticNetwork:public Utils::NeuralNetwork
    {
    public:
        GeneticNetwork(const Utils::NetworkSettings& settings);
        GeneticNetwork();

        SnakeMove feedForward(const std::vector<double>& input) const;
        
        void setFitness(double value);
        void updateFitness(double value);
        double getFitness()const;

        void setSelectionProb(double prob);
        double getSelectionProb() const;

        void mutateWeights(double mutateProb);
        void crossover(GeneticNetwork& otherNetwork);



    private:
        double m_fitness;
        double m_selectionProb;
        mutable std::mt19937 m_merseneTwister;
    };
}}
