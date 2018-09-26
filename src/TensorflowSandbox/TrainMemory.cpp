#include "TrainMemory.h"

using namespace AI::QLearning;

void TrainMemory::RegisterMemory(
	const std::vector<double>& state,
	int action,
	double reward,
	const std::vector<double>& nextState)
{
	states.push_back(state);
	actions.push_back(action);
	rewards.push_back(reward);
	nextStates.push_back(nextState);
	
	nbOfMemories++;
}

std::vector<float> TrainMemory::GetDiscountedRewards(double qDiscountFactor)
{
	std::vector<float> discountedRewards;
	discountedRewards.reserve(rewards.size());
	
	float lastReward = 0;
	for (auto it = rewards.rbegin(); it != rewards.rend(); it++)
	{
		lastReward = lastReward * qDiscountFactor + (*it);
		discountedRewards.push_back(lastReward);
	}
	std::reverse(discountedRewards.begin(), discountedRewards.end());
	
	return discountedRewards;
}
