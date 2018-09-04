#pragma once

#include <functional>

namespace AI { namespace QLearning
{
	typedef std::function<double ()> DblFunc;
	
	struct QOptions
	{
		// Hyperparameters.
		double learningRate = 0.1;
		double qDiscountFactor = 0.99;
		
		// Tweeking episodes.
		DblFunc numEpisodes;
		DblFunc maxNumSteps;
		
		// Random action chance.
		double maxRandActionChance = 1;
		double minRandActionChance = 0.00;
		double randActionDecayFactor = 1.0 / 100000;
		
		// Rewards.
		DblFunc foodReward = []() { return 1.0; };
		DblFunc dieReward = []() { return -1.0; };
		DblFunc stepReward = []() { return -0.001; };
	};
}}
