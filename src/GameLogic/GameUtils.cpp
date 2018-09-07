#include "GameUtils.h"
#include "IPlayer.h"
#include <algorithm>

unsigned int IndexOfMove(const SnakeMove move)
{
	const auto it = std::find(
		IPlayer::possibleMoves.begin(),
		IPlayer::possibleMoves.end(),
		move);
	
	return std::distance(IPlayer::possibleMoves.begin(), it);
}
