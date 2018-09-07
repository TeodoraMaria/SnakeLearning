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

        const std::vector<float> feedForward(const std::vector<float>& input) const;

    protected:
        float getWeightAt(size_t layer, size_t weightIndex, size_t inputIndex)const;

        std::vector<float> m_weights;
    private:
        std::vector<float> singleForward(const std::vector<float>& input,size_t nextLayer)const;

        void setWeightsSize();
        void initWieghts();

        const NetworkSettings& m_settings;
        std::vector<size_t> m_layerOffset;
    };
}