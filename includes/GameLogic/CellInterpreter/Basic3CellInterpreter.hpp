#pragma once

#include "ICellInterpreter.hpp"

namespace GameLogic { namespace CellInterpreter
{
	/*
	** 0 - Empty
	** 1 - Obstacle = Wall or any snake.
	** 2 - Food.
	*/
	
	class Basic3CellInterpreter : public ICellInterpreter
	{
		size_t NbOfInterpretableParts() const override;
		size_t InterpretCell(int playerId, BoardPart boardPart) const override;
	};
}}
