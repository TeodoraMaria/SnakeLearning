#include <vector>


namespace Utils
{
    struct NetworkSettings
    {
        size_t m_inputs=0;
        std::vector<size_t> m_hiddenLayersSizes = {0};
    };

    class NeuralNetwork
    {
    public:
        NeuralNetwork(const NetworkSettings& settings);
        NeuralNetwork();
        ~NeuralNetwork();

        const std::vector<float> feedForward(const std::vector<float>& input) const;
        void initWeights();

        void setSettings(const NetworkSettings& settings);

        std::vector<float> getWeights() const;

    protected:
        float getWeightAt(size_t layer, size_t weightIndex, size_t inputIndex)const;

        std::vector<float> m_weights;
        std::vector<float> m_biases;
    private:
        std::vector<float> singleForward(const std::vector<float>& input,size_t nextLayer)const;
        float sigmoid(float x) const;
        void setWeightsSize();

        NetworkSettings m_settings;
        std::vector<size_t> m_layerOffset;        
    };
}