#include "NeuralNetwork.h"

using namespace Utils;

NeuralNetwork::NeuralNetwork(const NetworkSettings& settings):
    m_settings(settings),
    m_layerOffset(std::vector<size_t>(settings.m_hiddenLayersSizes.size()))
{
    setWeightsSize();
    initWieghts();
}

NeuralNetwork::~NeuralNetwork()
{

}

const std::vector<float> NeuralNetwork::feedForward(std::vector<float> input) const
{
    auto result = singleForward(input, 0);
    return result;
}

std::vector<float> NeuralNetwork::singleForward(std::vector<float> input, size_t nextLayer) const
{
    std::vector<float> result(m_settings.m_hiddenLayersSizes[nextLayer]);

    for (size_t nextLayerIndex = 0; nextLayerIndex < result.size(); nextLayerIndex++) {
        size_t prod = 0;
        for (size_t layerIndex = 0; layerIndex < input.size(); layerIndex++) {

            prod += getWeightAt(0, nextLayerIndex, layerIndex)* input[layerIndex];
        }
        result[nextLayerIndex] = prod;
    }

    return result;
}

float NeuralNetwork::getWeightAt(size_t layer, size_t inputIndex, size_t weightIndex) const
{
    size_t layerSize = m_settings.m_hiddenLayersSizes[layer];
    //TODO: make it work for more layers;
    size_t index = m_layerOffset[layer] + inputIndex*layerSize + weightIndex;
    return m_weights[index];
}

void NeuralNetwork::setWeightsSize()
{
    size_t totalWeights = 0;

    totalWeights += m_settings.m_inputs * m_settings.m_hiddenLayersSizes[0];
    m_layerOffset[0] = 0;

    size_t numOfLayers = m_settings.m_hiddenLayersSizes.size();

    for (size_t i = 0; i < numOfLayers - 1; ++i) {
        m_layerOffset[i + 1] = totalWeights;
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
