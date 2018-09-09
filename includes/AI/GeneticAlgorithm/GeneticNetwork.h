#pragma once

#include <vector>
#include <GameLogic/GameBoard.h>
#include "Utils/NeuralNetwork.h"



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

        void selectForCrossOver(bool selected);
        bool isSelectedForCrossOver();

        void mutateWeights(float mutateProb);
        void crossover(GeneticNetwork& otherNetwork);

    private:
        double m_fitness;
        double m_selectionProb;
        bool m_selectedForCrossover;
    };
}}
