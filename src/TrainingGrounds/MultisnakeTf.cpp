#include "TensorflowSandbox/NeuralQAgent.hpp"
#include "TensorflowSandbox/NeuralQTrainer.hpp"

void MultisnakeTfMain()
{
	auto tfTrainer = AI::QLearning::NeuralQTrainer();
	tfTrainer.Train();
}
