#pragma once

#include <functional>
#include <random>

namespace AI { namespace QLearning
{
	typedef std::function<double (int episode)> DblFunc;
	typedef std::function<size_t (int episode)> SizeTFunc;
	
	struct QOptions
	{
		// Hyperparameters.
		double learningRate = 0.1;
		double qDiscountFactor = 0.99;
		
		// Tweeking episodes.
		size_t numEpisodes;
		std::function<size_t (int episode)> maxNumSteps;
		
		// Random action chance.
		double maxRandActionChance = 1;
		double minRandActionChance = 0.00;
		double randActionDecayFactor = 1.0 / 100000;
		SizeTFunc maxStepsWithoutFood = [](int episode) { return 150; };
		
		// Rewards.
		DblFunc foodReward = [](int episode) { return 1.0; };
		DblFunc dieReward = [](int episode) { return -1.0; };
		DblFunc stepReward = [](int episode) { return -0.001; };
		
		std::function<double (std::mt19937& merseneTwister)> tabInitializer =
			[](auto& merseneTwister) { return 0.0; };
	};
}}
