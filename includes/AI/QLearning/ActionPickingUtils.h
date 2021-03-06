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
		bool shuffleEquals = true,
		double eps = 0.000001);
	
	/*
	** Instead of choosing a random action, add some noise to the table.
	*/
	
	int PickActionAdditiveNoise(
		std::vector<double> actionsQ,
		double noise,
		std::mt19937& merseneTwister,
		double eps = 0.000001);
}}}
