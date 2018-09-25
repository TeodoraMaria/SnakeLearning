#include "SupervisedBot.h"
#include "SupervisedTrainer.h"
#include <algorithm>

using namespace AI::Supervised;


AI::Supervised::SupervisedBot::SupervisedBot(const SupervisedNetwork::Settings& settings)
{
	m_network = new SupervisedNetwork(settings);
}


SnakeMove AI::Supervised::SupervisedBot::GetNextAction(const GameState & gameState)
{
	auto input = ExtractInput(gameState);
	auto output = m_network->Evaluate(input);
	return InterpretOutput(output);
}

SupervisedNetwork * AI::Supervised::SupervisedBot::GetNetwork() const
{
	return m_network;
}

std::vector<double> AI::Supervised::SupervisedBot::ExtractInput(const GameState & gameState) const
{
	auto fieldOfView = gameState.GetFieldOfView(gameState.GetSnake(GetSnakeNumber()), 3, 3);
	std::vector<double> input(fieldOfView.begin(), fieldOfView.end());
	for (auto& elem : input)
	{
		if (elem == 0)
		{
			elem = 0;
		}		
		else if (elem == 2)
		{
			elem = 1;
		}
		else
		{
			elem = -1;
		}
	}
	return input;
}

SnakeMove AI::Supervised::SupervisedBot::InterpretOutput(std::vector<double> output) const
{

	/*auto it = std::max_element(output.begin(), output.end());
	auto index = std::distance(output.begin(), it);*/
	if(index==0)
		return SnakeMove::FORWARD;
	if (index == 1)
		return SnakeMove::LEFT;
	if (index == 2)
		return SnakeMove::RIGHT;
	return SnakeMove::FORWARD;
}
