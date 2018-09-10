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
		void SetPlayer(std::shared_ptr<IPlayer> player);
	
		size_t NbOfInterpretableParts() const override;
		size_t InterpretCell(BoardPart boardPart) const override;
	
	private:
		std::shared_ptr<IPlayer> m_player;
	};
}}
