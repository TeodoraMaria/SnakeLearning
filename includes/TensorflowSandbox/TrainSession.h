#pragma once

#include "TrainMemory.h"

namespace AI { namespace QLearning
{
	struct TrainSession
	{
		double episodeReward = 0;
		int stepsWithoutFood = 0;
		
		TrainMemory trainMemory;
	};
}}
