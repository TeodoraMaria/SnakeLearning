#pragma once

#include <vector>
#include <GameLogic/GameBoard.h>
#include "Utils/NeuralNetwork.h"

namespace AI{namespace GeneticAlgorithm{
   
    class GeneticNetwork:public Utils::NeuralNetwork
    {
    public:
        GeneticNetwork();

        SnakeMove feedForward(const std::vector<double>& input) const;
        void initWithRandomValues();

        void updateFitness(double value);
        double getFitness()const;

        void setSelectionProb(double prob);
        double getSelectionProb() const;

        void selectForCrossOver(bool selected);
        bool isSelectedForCrossOver();

    private:
        //TODO: implement
        //add a vector with 1 or more matrices with weights; used for feedforwards
        //m_weigths;
        double m_fitness;
        double m_selectionProb;
        bool m_selectedForCrossover;
    };
}}
