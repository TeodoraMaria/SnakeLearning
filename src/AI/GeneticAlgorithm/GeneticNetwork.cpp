#include "GeneticNetwork.h"
#include "Utils/MathUtils.h"
#include <algorithm>


using namespace AI::GeneticAlgorithm;

GeneticNetwork::GeneticNetwork(const Utils::NetworkSettings& settings):
    m_fitness(0.0),
    m_selectionProb(0.0),
    m_selectedForCrossover(false),
    NeuralNetwork(settings)
{
 
}

GeneticNetwork::GeneticNetwork()
{

}

SnakeMove GeneticNetwork::feedForward(const std::vector<double>& input) const
{
    //TODO: something with bias;
    std::vector<float> floatInput(input.begin(),input.end());
    floatInput.push_back(1);

    const auto& result = Utils::NeuralNetwork::feedForward(floatInput);

    SnakeMove bestMove;
    size_t index=std::distance(result.begin(), std::max_element(result.begin(), result.end()));

    switch (index) {
        case 0: {
            bestMove = SnakeMove::FORWARD;
            break; 
        }
        case 1: {
            bestMove = SnakeMove::RIGHT;
            break;
        }
        case 2: {
            bestMove = SnakeMove::DOWN;
            break;
        }
        case 3: {
            bestMove = SnakeMove::LEFT;
            break;
        }
        default:
            break;
    }

    return bestMove;
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

void GeneticNetwork::mutateWeights(float mutateProb)
{
    for (auto& weight : m_weights) {
        float mutateValue = Utils::Math::randomDouble(0.00001, 1.0);

        if (mutateValue < mutateProb) {
            weight = Utils::Math::randomDouble(0.00001, 1.0);
        }
    }
}

void GeneticNetwork::crossOver(GeneticNetwork & otherNetwork)
{
    size_t numOfWeights = Utils::Math::randomNumber<size_t>(0, m_weights.size());
    
    std::swap_ranges(m_weights.begin(), m_weights.begin() + numOfWeights, otherNetwork.m_weights.begin());
}
