#include "NeuralNetwork.h"
#include "Utils/MathUtils.h"
#include <iostream>

using namespace Utils;

template<class T>
NeuralNetwork<T>::NeuralNetwork(const NetworkSettings& settings):
    m_settings(settings),
    m_layerOffset(std::vector<size_t>(settings.m_hiddenLayersSizes.size()))
{
    setWeightsSize();
    initRandomWeights();
}

template<class T>
NeuralNetwork<T>::~NeuralNetwork()
{

}

template<class T>
const std::vector<T> NeuralNetwork<T>::feedForward(const std::vector<T>& input) const
{
    std::vector<T> result = singleForward(input, 0);

    for (size_t layer = 1; layer < m_settings.m_hiddenLayersSizes.size(); layer++) {
        std::vector<T>& temp = singleForward(result, layer);
        result = temp;
    }
    return result;
}

template<class T>
std::vector<T> NeuralNetwork<T>::singleForward(const std::vector<T>& input, size_t nextLayer) const
{
    std::vector<T> result(m_settings.m_hiddenLayersSizes[nextLayer]);

    for (size_t nextLayerIndex = 0; nextLayerIndex < result.size(); nextLayerIndex++) {
        float prod = 0;
        for (size_t layerIndex = 0; layerIndex < input.size(); layerIndex++) {

            prod += getWeightAt(nextLayer, nextLayerIndex, layerIndex)* input[layerIndex];
        }
        result[nextLayerIndex] = sigmoid(prod);
        std::cout << result[nextLayerIndex]<<" ";
    }
    std::cout << std::endl;
    return result;
}

template<class T>
T NeuralNetwork<T>::sigmoid(T x) const
{
    return 1.0f / (1.0f + std::exp(-x));
}

template<class T>
float NeuralNetwork<T>::getWeightAt(size_t layer, size_t inputIndex, size_t weightIndex) const
{
    size_t layerSize=0;
    if (layer == 0) {
        layerSize = m_settings.m_inputs;
    } else {
        layerSize = m_settings.m_hiddenLayersSizes[layer - 1];
    }

    //TODO: make it work for more layers;
    size_t index = m_layerOffset[layer] + inputIndex*layerSize + weightIndex;
    return m_weights[index];
}

template<class T>
void NeuralNetwork<T>::setWeightsSize()
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

template<class T>
void NeuralNetwork<T>::initRandomWeights()
{
    for (auto& weight : m_weights) {
        weight = static_cast <T> (rand()) / static_cast <T> (RAND_MAX);
    }
}


template class NeuralNetwork<float>;
template class NeuralNetwork<double>;
