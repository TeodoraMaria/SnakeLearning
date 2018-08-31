#pragma once

#include "AI/ITrainer.hpp"
#include <cstddef>
#include <random>

namespace AI{ namespace QLearning
{
	class TabularTrainer : public AI::ITrainer
	{
	public:
		static int GetAction(
			const std::vector<double>& qActions,
			double noise,
			std::mt19937& merseneTwister);
	
		IPlayer* Train() override;
	};
}}
