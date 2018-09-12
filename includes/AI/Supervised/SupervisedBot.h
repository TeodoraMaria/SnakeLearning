#pragma once

#include "GameLogic/IPlayer.h"
#include "Utils/NeuralNetwork.h"

namespace AI {
	namespace Supervised
	{
		class SupervisedBot : public IPlayer
		{
		public:

			SupervisedBot(const Utils::NeuralNetwork& network);
			SnakeMove GetNextAction(const GameState& gameState) const override;

		private:
			std::vector<float> ExtractInput(const GameState& gameState) const;
			SnakeMove InterpretOutput(std::vector<float>) const;

			Utils::NeuralNetwork m_network;
		};
	}
}