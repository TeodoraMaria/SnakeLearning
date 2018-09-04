#include "ActionPickingUtils.h"
#include "Utils/MathUtils.h"

typedef std::uniform_int_distribution<int> IntDistrib;

static size_t CountElementsWithValue(
	const std::vector<double>& tab,
	const double targetValue)
{
	return std::count_if(
		tab.begin(),
		tab.end(),
		[&](const auto& value)
		{
			return ::Utils::Math::Approx(value, targetValue);
		});
}

/*
** Randomly get the index of which max element will be choosen.
*/

static int GetIndexOfOneMax(
	const std::vector<double>& actionsQ,
	const double maxActionQ,
	std::mt19937& merseneTwister)
{
	const auto maxElementsCount = CountElementsWithValue(
		actionsQ,
		maxActionQ);

	if (maxElementsCount == 1)
		return 0;
	else
	{
		auto maxActionDistrib = IntDistrib(0, maxElementsCount - 1);
		return maxActionDistrib(merseneTwister);
	}
}

static int GetIndexOfNthMaxElement(
	const std::vector<double>& actionsQ,
	const double maxActionQ,
	int maxActionIndex)
{
	for (auto i = 0u; i < actionsQ.size(); i++)
	{
		if (::Utils::Math::Approx(actionsQ[i], maxActionQ))
		{
			if (maxActionIndex == 0)
				return i;
			
			maxActionIndex--;
		}
	}
	throw;
}

/*
** Get the index of an action from the given quality table.
** If the the max qualities are equal, get a random one from those. This can
** be controlled by shuffleEquals.
*/

int AI::QLearning::Utils::PickAction(
	const std::vector<double>& actionsQ,
	const double noise,
	std::mt19937& merseneTwister,
	const bool shuffleEquals)
{
	auto chanceDistrib = std::uniform_real_distribution<double>(0, 1.0);
	
	// Random action.
	if (chanceDistrib(merseneTwister) < noise)
	{
		auto actionDistrib = IntDistrib(0, actionsQ.size() - 1);
		return actionDistrib(merseneTwister);
	}
	
	// Greedy pick.
	const auto maxActionQ = *std::max_element(
		actionsQ.cbegin(),
		actionsQ.cend());

	auto maxActionIndex = (!shuffleEquals) ? 0 :
		GetIndexOfOneMax(actionsQ, maxActionQ, merseneTwister);

	return GetIndexOfNthMaxElement(actionsQ, maxActionQ, maxActionIndex);
}
