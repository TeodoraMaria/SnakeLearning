#pragma once

#include <vector>

namespace AI { namespace QLearning
{
	struct TrainMemory
	{
		std::vector<std::vector<double>> states;
		std::vector<int> actions;
		std::vector<double> rewards;
		std::vector<std::vector<double>> nextStates;
		
		int nbOfMemories = 0;
		
		void RegisterMemory(
			const std::vector<double>& state,
			int action,
			double reward,
			const std::vector<double>& nextState);
		
		std::vector<float> GetDiscountedRewards(double qDiscountFactor);
	};
}}
