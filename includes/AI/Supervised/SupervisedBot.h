#pragma once

#include "GameLogic/IPlayer.h"
#include "SupervisedNetwork.h"

using namespace AI::Supervised;

namespace AI {
	namespace Supervised
	{
		class SupervisedBot : public IPlayer
		{
		public:

			SupervisedBot();
			SnakeMove GetNextAction(const GameState& gameState) override;

		private:
			std::vector<double> ExtractInput(const GameState& gameState) const;
			SnakeMove InterpretOutput(std::vector<double>) const;

			SupervisedNetwork m_network;
		};
	}
}