#include "..\..\..\includes\AI\Supervised\SupervisedBot.h"

AI::Supervised::SupervisedBot::SupervisedBot(const Utils::NeuralNetwork & network):m_network(network)
{
}

SnakeMove AI::Supervised::SupervisedBot::GetNextAction(const GameState & gameState) const
{
	auto input = ExtractInput(gameState);
	const auto output = m_network.feedForward(input);
	return InterpretOutput(output);
}

std::vector<float> AI::Supervised::SupervisedBot::ExtractInput(const GameState & gameState) const
{
	auto fieldOfView = gameState.GetFieldOfView(gameState.GetSnake(GetSnakeNumber()), 5, 5);
	std::vector<float> input(fieldOfView.begin(), fieldOfView.end());
	return input;
}

SnakeMove AI::Supervised::SupervisedBot::InterpretOutput(std::vector<float>) const
{
	//DO SOME STUFF HERE
	return SnakeMove::FORWARD;
}
