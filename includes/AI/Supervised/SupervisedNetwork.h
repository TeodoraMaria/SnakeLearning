#pragma once

#include <stdint.h>
#include <vector>
#include <cmath>
#include <json.hpp>

namespace AI {
	namespace Supervised {
		

		enum class ActivationFunctionType
		{
			Sigmoid
		};
		class SupervisedNetwork {
			friend class SupervisedTrainer;

			//-------------------------------------------------------------------------

			inline static double SigmoidActivationFunction(double x)
			{
				return 1.0 / (1.0 + std::exp(-x));
				//return x > 0 ? x : 0.01*x; //leaky relu
			}

			inline static int32_t ClampOutputValue(double x)
			{
				if (x < 0.5) return 0;
				//else if (x > 0.6) return 1;
				else return 1;
			}

		public:

			struct Settings
			{
				uint32_t                        m_numInputs;
				uint32_t                        m_numHidden;
				uint32_t                        m_numOutputs;
			};

		public:
			SupervisedNetwork() {}
			SupervisedNetwork(Settings const& settings);
			SupervisedNetwork(Settings const& settings, std::vector<double> const& weights);

			std::vector<double> const& Evaluate(std::vector<double> const& input);

			std::vector<double> const& GetInputHiddenWeights() const { return m_weightsInputHidden; }
			std::vector<double> const& GetHiddenOutputWeights() const { return m_weightsHiddenOutput; }

			friend void from_json(const nlohmann::json& j, SupervisedNetwork& net);
			friend void to_json(nlohmann::json& j, const SupervisedNetwork& net);

			void Initialize(const SupervisedNetwork& net);

		private:

			void InitializeNetwork();
			void InitializeWeights();
			void LoadWeights(std::vector<double> const& weights);

			int32_t GetInputHiddenWeightIndex(int32_t inputIdx, int32_t hiddenIdx) const { return inputIdx * m_numHidden + hiddenIdx; }
			int32_t GetHiddenOutputWeightIndex(int32_t hiddenIdx, int32_t outputIdx) const { return hiddenIdx * m_numOutputs + outputIdx; }
		
		private:

			int32_t                 m_numInputs;
			int32_t                 m_numHidden;
			int32_t                 m_numOutputs;

			std::vector<double>     m_inputNeurons;
			std::vector<double>     m_hiddenNeurons;
			std::vector<double>     m_outputNeurons;

			std::vector<double>    m_clampedOutputs;

			std::vector<double>     m_weightsInputHidden;
			std::vector<double>     m_weightsHiddenOutput;
		};
	}
}
