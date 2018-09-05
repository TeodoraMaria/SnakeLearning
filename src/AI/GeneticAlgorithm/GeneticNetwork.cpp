#include "GeneticNetwork.h"

using namespace AI::GeneticAlgorithm;

GeneticNetwork::GeneticNetwork():m_fitness(0.0)
{
    initWithRandomValues();
}

SnakeMove GeneticNetwork::feedForward(const std::vector<int>& input) const
{
    //TODO: implement get max value from matrix mult;
    return SnakeMove::FORWARD;
}

void GeneticNetwork::initWithRandomValues()
{
    //TODO: put random values in the matrix;
}

void GeneticNetwork::updateFitness(const double value)
{
    m_fitness += value;
}

double GeneticNetwork::getFitness() const
{
    return m_fitness;
}
