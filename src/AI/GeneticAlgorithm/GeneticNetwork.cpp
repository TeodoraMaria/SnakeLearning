#include "GeneticNetwork.h"

using namespace AI::GeneticAlgorithm;

GeneticNetwork::GeneticNetwork():m_fitness(0.0),m_selectionProb(0.0),m_selectedForCrossover(false)
{
    initWithRandomValues();
}

SnakeMove GeneticNetwork::feedForward(const std::vector<double>& input) const
{
    //TODO: implement get max value from matrix mult;
    return SnakeMove::FORWARD;
}

void GeneticNetwork::initWithRandomValues()
{
    //TODO: put random values in the matrix;
}

void GeneticNetwork::updateFitness( double value)
{
    m_fitness += value;
}

double GeneticNetwork::getFitness() const
{
    return m_fitness;
}

void GeneticNetwork::setSelectionProb(double prob)
{
    m_selectionProb = prob;
}

double GeneticNetwork::getSelectionProb() const
{
    return m_selectionProb;
}

void GeneticNetwork::selectForCrossOver(bool selected)
{
    m_selectedForCrossover = selected;
}

bool GeneticNetwork::isSelectedForCrossOver()
{
    return m_selectedForCrossover;
}
