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

const std::vector<float>& NeuralNetwork::feedForward(std::vector<float> input) const
{
    // TODO: insert return statement here

    return std::vector<float>();

}

float NeuralNetwork::getWeightAt(size_t layer) const
{
    return 0.0f;
}

void NeuralNetwork::setWeightsSize()
{
    size_t totalWeights = 0;

    totalWeights += m_settings.m_inputs * m_settings.m_hiddenLayersSizes[0];

    size_t numOfLayers = m_settings.m_hiddenLayersSizes.size();

    for (size_t i = 0; i < numOfLayers - 1; ++i) {
        totalWeights += m_settings.m_hiddenLayersSizes[i] * m_settings.m_hiddenLayersSizes[i + 1];
    }

    totalWeights += m_settings.m_hiddenLayersSizes[numOfLayers - 1] * m_settings.m_outputs;

    m_weights.resize(totalWeights);
}
