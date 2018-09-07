#include "TabularActionPicker.hpp"
#include "ActionPickingUtils.h"
#include "GymEnv/StateExtractor.hpp"
#include "GameLogic/IPlayer.h"
#include <assert.h>

using namespace AI::QLearning;

TabularActionPicker::TabularActionPicker(
	size_t nbOfInterpretableBoardParts,
	double actionQEqualityEps,
	std::shared_ptr<QTabularBrains> brains
) :
	m_nbOfInterpretableBoardParts(nbOfInterpretableBoardParts),
	m_actionQEqualityEps(actionQEqualityEps),
	m_brains(brains),
	m_merseneTwister(std::random_device()())
{
}

SnakeMove TabularActionPicker::PickMove(
	const std::vector<double>& observations,
	const double noise)
{
	const State state = GymEnv::Utils::StateExtractor::BinaryVectorToNumber(
		observations,
		m_nbOfInterpretableBoardParts);
	
	const auto actionIndx = Utils::PickAction(
		m_brains->GetQTabRow(state),
		noise,
		m_merseneTwister,
		true,
		m_actionQEqualityEps);
	
	assert(actionIndx < IPlayer::possibleMoves.size());
	return IPlayer::possibleMoves[actionIndx];
}

