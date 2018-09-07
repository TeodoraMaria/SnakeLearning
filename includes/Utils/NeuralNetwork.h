#include <vector>


namespace Utils
{
    struct NetworkSettings
    {
        size_t m_inputs;
        std::vector<size_t> m_hiddenLayersSizes;
        size_t m_outputs;
    };

    class NeuralNetwork
    {
    public:
        NeuralNetwork(const NetworkSettings& settings);
        ~NeuralNetwork();

        const std::vector<float>& feedForward(std::vector<float> input) const;

        float getWeightAt(size_t layer)const;

    protected:

        std::vector<float> m_weights;
    private:

        void setWeightsSize();

        const  NetworkSettings& m_settings;
    };
}