#pragma once

#include "GameLogic/IPlayer.h"

namespace AI
{
	class ITrainer
	{
	public:
		virtual IPlayer* Train() = 0;
	};
}
