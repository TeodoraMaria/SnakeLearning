#include <vector>


namespace Utils
{
    struct NetworkSettings
    {
        size_t m_inputs;
        std::vector<size_t> m_hiddenLayersSizes;
    };
    template<typename T>
    class NeuralNetwork
    {
    public:
        NeuralNetwork(const NetworkSettings& settings);
        ~NeuralNetwork();

        const std::vector<T> feedForward(const std::vector<T>& input) const;

    protected:
        float getWeightAt(size_t layer, size_t weightIndex, size_t inputIndex)const;

        std::vector<T> m_weights;
    private:
        std::vector<T> singleForward(const std::vector<T>& input,size_t nextLayer)const;
        T sigmoid(T x) const;

        void setWeightsSize();
        void initRandomWeights();

        const NetworkSettings& m_settings;
        std::vector<size_t> m_layerOffset;
    };
}