#include "GeneticNetwork.h"
#include "Utils/MathUtils.h"
#include "AI/QLearning/ActionPickingUtils.h"
#include <algorithm>
#include <iostream>
#include <algorithm>

using namespace AI::GeneticAlgorithm;

GeneticNetwork::GeneticNetwork(const Utils::NetworkSettings& settings):
    m_fitness(0.0),
    m_selectionProb(0.0),
    NeuralNetwork(settings),
    m_merseneTwister(std::random_device()())
{
    
}

GeneticNetwork::GeneticNetwork():
    m_fitness(0.0),
    m_selectionProb(0.0),
    NeuralNetwork(Utils::NetworkSettings()),
    m_merseneTwister(std::random_device()())
{
    
}

SnakeMove GeneticNetwork::feedForward(const std::vector<double>& input) const
{
    //TODO: something with bias;
    std::vector<float> floatInput(input.begin(),input.end());
    floatInput.push_back(1);

    const auto& result = Utils::NeuralNetwork::feedForward(floatInput);

    std::vector<double> doubleInput(result.begin(), result.end());

    SnakeMove bestMove;
    size_t index=std::distance(result.begin(), std::max_element(result.begin(), result.end()));
    //size_t index = AI::QLearning::Utils::PickAction(doubleInput, 0, m_merseneTwister,true,0.001);

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
            bestMove = SnakeMove::LEFT;
            break;
        }
        case 3: {//TODO: remove
            bestMove = SnakeMove::DOWN;
            break;
        }
        default:
            break;
    }

    return bestMove;
}

void AI::GeneticAlgorithm::GeneticNetwork::setFitness(double value)
{
    m_fitness = value;
}

void GeneticNetwork::updateFitness( double value)
{
    m_fitness += value;
}

double GeneticNetwork::getFitness() const
{
    return m_fitness *m_fitness * m_fitness;
}

void GeneticNetwork::setSelectionProb(double prob)
{
    m_selectionProb = prob;
}

double GeneticNetwork::getSelectionProb() const
{
    return m_selectionProb;
}

void GeneticNetwork::mutateWeights(double mutateProb)
{
    for (auto& weight : m_weights) {
        double mutateValue = Utils::Math::randomDouble(0.00001, 1.0);
        
        if (mutateValue < mutateProb) {
            weight += Utils::Math::randomDouble(-0.1, 0.1);
            weight = std::max(-1.0f, std::min(1.0f, weight));
        }
        
        
        /*
        if (mutateValue < mutateProb) {
            weight = Utils::Math::randomDouble(-1.0, 1.0);
        }
        */
    }
}

void GeneticNetwork::crossover(GeneticNetwork & otherNetwork)
{
    //size_t numOfWeights = Utils::Math::randomNumber<size_t>(0, m_weights.size());
    //std::swap_ranges(m_weights.begin(), m_weights.begin() + numOfWeights, otherNetwork.m_weights.begin());
    for (size_t i = 0; i < m_weights.size(); i++) {
        size_t randomNumber= Utils::Math::randomNumber<size_t>(0, 100);
        if (randomNumber < 50) {
            std::swap(m_weights[i], otherNetwork.m_weights[i]);
        }
    }
}
