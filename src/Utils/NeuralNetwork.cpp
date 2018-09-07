#include "NeuralNetwork.h"

using namespace Utils;

NeuralNetwork::NeuralNetwork(const NetworkSettings& settings)
    :m_settings(settings)
{
    setWeightsSize();
}

NeuralNetwork::~NeuralNetwork()
{

}

const std::vector<float> NeuralNetwork::feedForward(std::vector<float> input) const
{
    // TODO: insert return statement here

    for (size_t i = 0; i < input.size(); i++) {

        for (size_t j = 0; j < m_settings.m_hiddenLayersSizes[0]; i++) {

        }
    }

    return std::vector<float>();

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
