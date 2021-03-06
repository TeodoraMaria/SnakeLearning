#include "NeuralNetwork.h"
#include "Utils/MathUtils.h"
#include <iostream>
#include <cmath>

using namespace Utils;

NeuralNetwork::NeuralNetwork(const NetworkSettings& settings):
    m_settings(settings),
    m_layerOffset(std::vector<size_t>(settings.m_hiddenLayersSizes.size()))
{
    setWeightsSize();
    initWeights();
}

NeuralNetwork::NeuralNetwork()
{
    
}

NeuralNetwork::~NeuralNetwork()
{

}

const std::vector<float> NeuralNetwork::feedForward(std::vector<float>& input) const
{
    auto result = singleForward(input, 0);

    for (size_t i = 1; i < m_settings.m_hiddenLayersSizes.size(); i++) {
        auto temp = singleForward(result, i);
        result = temp;
    }
    return result;
}

std::vector<float> NeuralNetwork::singleForward(std::vector<float>& input, size_t nextLayer) const
{
    std::vector<float> result(m_settings.m_hiddenLayersSizes[nextLayer]);

    for (size_t nextLayerIndex = 0; nextLayerIndex < result.size(); nextLayerIndex++) {
        float prod = 0;
        for (size_t layerIndex = 0; layerIndex < input.size(); layerIndex++) {

            prod += getWeightAt(nextLayer, nextLayerIndex, layerIndex)* input[layerIndex];
        }
        result[nextLayerIndex] = activate(prod);
    }
    return result;
}

float NeuralNetwork::sigmoid(float x) const
{
    return 1.0f / (1.0f + std::exp(-x));
}

float Utils::NeuralNetwork::relu(float x) const
{
	return x <= 0 ? 0 : x;
}

float Utils::NeuralNetwork::activate(float x) const
{
	if (m_settings.m_activationFc == ActivationFunction::RELU)
		return relu(x);
	return sigmoid(x);
}

float NeuralNetwork::getWeightAt(size_t layer, size_t inputIndex, size_t weightIndex) const
{
    size_t layerSize=0;
    if (layer == 0) {
        layerSize = m_settings.m_inputs;
    } else {
        layerSize = m_settings.m_hiddenLayersSizes[layer - 1];
    }

    
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

void NeuralNetwork::initWeights()
{
    for (auto& weight : m_weights) {
        //weight = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        weight= Utils::Math::randomDouble(-1.0, 1.0);
    }
}

void NeuralNetwork::setSettings(const NetworkSettings & settings)
{
    m_settings = settings;
}

size_t Utils::NeuralNetwork::GetNumberOfInputs()
{
	return m_settings.m_inputs;
}

std::vector<float> Utils::NeuralNetwork::getWeights() const
{
    return m_weights;
}

void Utils::NeuralNetwork::setWeights(const std::vector<float>& weights)
{
    m_weights = weights;
}

NetworkSettings NeuralNetwork::getNetworkSettings()const
{
    return m_settings;
}
