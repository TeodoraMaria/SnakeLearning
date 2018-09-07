#include <vector>


namespace Utils
{
    struct NetworkSettings
    {
        size_t m_inputs;
        std::vector<size_t> m_hiddenLayersSizes;
    };

    class NeuralNetwork
    {
    public:
        NeuralNetwork(const NetworkSettings& settings);
        ~NeuralNetwork();

        const std::vector<float> feedForward(std::vector<float> input) const;

        float getWeightAt(size_t layer,size_t line,size_t col)const;

    protected:

        std::vector<float> m_weights;
    private:

        void setWeightsSize();

        const NetworkSettings& m_settings;
    };
}