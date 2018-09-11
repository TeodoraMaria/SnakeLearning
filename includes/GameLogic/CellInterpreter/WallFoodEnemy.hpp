#pragma once

#include "GameLogic/IPlayer.h"
#include "ICellInterpreter.hpp"

namespace GameLogic { namespace CellInterpreter
{
	/*
	** 0 - Empty
	** 1 - Wall
	** 2 - Food
	** 3 - His body
	** 4 - Enemy body
	*/
	
	class WallFoodEnemy : public ICellInterpreter
	{
	public:
		size_t NbOfInterpretableParts() const override;
		size_t InterpretCell(int playerId, BoardPart boardPart) const override;
	};
}}
