#include "TensorflowSandbox/NeuralQAgent.hpp"
#include "TensorflowSandbox/NeuralQTrainer.hpp"

void MultisnakeTfMain()
{
	auto tfTrainer = AI::QLearning::NeuralQTrainer();
	
	auto trainCallbacks = AI::ITrainer::TrainCallbacks();
	trainCallbacks.numEpisodes = 5000;
	
	tfTrainer.Train(trainCallbacks);
}
