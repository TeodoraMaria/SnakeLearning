#pragma once

#include "ICellInterpreter.hpp"

namespace GameLogic { namespace CellInterpreter
{
	/*
	** 0 - Empty
	** 1 - Wall
	** 2 - Food
	** 3 - Any snake body
	*/
	
	class WallFoodBody : public ICellInterpreter
	{
		size_t NbOfInterpretableParts() const override;
		size_t InterpretCell(int playerId, BoardPart boardPart) const override;
	};
}}

