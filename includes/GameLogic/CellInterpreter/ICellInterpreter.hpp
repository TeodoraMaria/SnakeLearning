#pragma once

#include "GameBoard.h"
#include <cstddef>

namespace GameLogic { namespace CellInterpreter
{
	struct UnrecognizedBoardPart : public std::exception {};

	class ICellInterpreter
	{
	public:
		// Number of cells this boy can interpret.
		virtual size_t NbOfInterpretableCells() const = 0;

		// Get the index of the board part type. For example, if it
		// can interpret 4 types of cells, then it could return:
		// 0 - Empty, 1 - Wall, 2 - Food, 3 - SnakeBody.
		virtual size_t InterpretCell(BoardPart boardPart) const = 0;
	};
}}
