#pragma once

#include <vector>
#include <random>

namespace AI { namespace QLearning { namespace Utils
{
	/*
	** shuffleEquals - pick a random action if max qualities are equal.
	*/
	
	int PickAction(
		const std::vector<double>& actionsQ,
		double noise,
		std::mt19937& merseneTwister,
		bool shuffleEquals = true);
}}}
