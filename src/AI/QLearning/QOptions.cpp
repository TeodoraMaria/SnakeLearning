#include "QOptions.h"

double AI::QLearning::QOptions::ComputeReward(int episode, int rawReward) const
{
	if (rawReward < 0)
	{
		return dieReward(episode);
	}
	else if (rawReward > 0)
	{
		return foodReward(episode);
	}
	else
	{
		return stepReward(episode);
	}
}
