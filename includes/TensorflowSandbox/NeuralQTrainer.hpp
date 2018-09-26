#pragma once

#include "AI/ITrainer.hpp"

namespace AI { namespace QLearning
{
	class NeuralQTrainer : public ITrainer
	{
	public:
		IPlayerPtr Train(TrainCallbacks callbacks) override;
	};
}}
