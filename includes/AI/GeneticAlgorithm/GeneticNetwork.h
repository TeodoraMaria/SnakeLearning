#pragma once;

#include <vector>
#include <GameLogic\GameBoard.h>

namespace AI{namespace GeneticAlgorithm{
   
    class GeneticNetwork
    {
    public:
        GeneticNetwork();

        SnakeMove feedForward(const std::vector<int>& input) const;
        void initWithRandomValues();
        void updateFitness(const double value);
        double getFitness()const;

    private:
        //TODO: implement
        //add a vector with 1 or more matrices with weights; used for feedforwards
        //m_weigths;
        double m_fitness;
    };
}}
