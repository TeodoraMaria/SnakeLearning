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
			SupervisedBot(const SupervisedNetwork::Settings& settings);
			SnakeMove GetNextAction(const GameState& gameState) override;
			SupervisedNetwork* GetNetwork() const;
			void SetNetwork(const SupervisedNetwork& net);

		private:
			std::vector<double> ExtractInput(const GameState& gameState) const;
			SnakeMove InterpretOutput(std::vector<double>) const;

			SupervisedNetwork* m_network;
		};
	}
}