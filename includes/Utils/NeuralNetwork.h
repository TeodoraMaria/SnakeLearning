#include <vector>
#include <cstddef>

namespace Utils
{
	enum class ActivationFunction {SIGMOID, RELU, SOFTMAX};

    struct NetworkSettings
    {
        size_t m_inputs=0;
        std::vector<size_t> m_hiddenLayersSizes = {0};
		ActivationFunction m_activationFc = ActivationFunction::SIGMOID;
    };

    class NeuralNetwork
    {
    public:
        NeuralNetwork(const NetworkSettings& settings);
        NeuralNetwork();
        ~NeuralNetwork();

        const std::vector<float> feedForward(std::vector<float>& input) const;
        void initWeights();

        void setSettings(const NetworkSettings& settings);
		size_t GetNumberOfInputs();
        NetworkSettings getNetworkSettings()const;

        std::vector<float> getWeights() const;
        void setWeights(const std::vector<float>& weights);

    protected:
        float getWeightAt(size_t layer, size_t weightIndex, size_t inputIndex)const;

        std::vector<float> m_weights;
        std::vector<float> m_biases;
    private:
        std::vector<float> singleForward(std::vector<float>& input,size_t nextLayer)const;
        float sigmoid(float x) const;
		float relu(float x) const;
		float softmax(float x) const;
		float activate(float x) const;
        void setWeightsSize();

        NetworkSettings m_settings;
        std::vector<size_t> m_layerOffset;        
    };
}