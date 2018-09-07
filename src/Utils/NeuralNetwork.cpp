#include "NeuralNetwork.h"

using namespace Utils;

NeuralNetwork::NeuralNetwork(const NetworkSettings& settings)
    :m_settings(settings)
{
    setWeightsSize();
    initWieghts();
}

NeuralNetwork::~NeuralNetwork()
{

}

const std::vector<float> NeuralNetwork::feedForward(std::vector<float> input) const
{
    std::vector<float> result(m_settings.m_hiddenLayersSizes.back());
    
    for (size_t j = 0; j < result.size(); j++) {
    size_t prod = 0;
        for (size_t k = 0; k < m_settings.m_inputs; k++) {
            prod += m_weights[j*result.size() + k] * input[k];
        }
    result[j] = prod;
    }

    return result;
}

float NeuralNetwork::getWeightAt(size_t layer, size_t line, size_t col) const
{
    size_t layerSize = m_settings.m_hiddenLayersSizes[layer];
    //TODO: make it work for more layers;
    return line*layerSize + col;
}

void NeuralNetwork::setWeightsSize()
{
    size_t totalWeights = 0;

    totalWeights += m_settings.m_inputs * m_settings.m_hiddenLayersSizes[0];

    size_t numOfLayers = m_settings.m_hiddenLayersSizes.size();

    for (size_t i = 0; i < numOfLayers - 1; ++i) {
        totalWeights += m_settings.m_hiddenLayersSizes[i] * m_settings.m_hiddenLayersSizes[i + 1];
    }

    m_weights.resize(totalWeights);
}

void NeuralNetwork::initWieghts()
{
    for (auto& weight : m_weights) {
        weight = 1;
    }
}
