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
	auto fieldOfView = gameState.GetFieldOfView(gameState.GetSnake(GetSnakeNumber()), 7, 7);
	std::vector<double> input(fieldOfView.begin(), fieldOfView.end());
	for (auto& elem : input)
	{
		if (elem == 0)
		{
			elem = 1;
		}
		else if (elem == 1)
		{
			elem = -2;
		}
		else if (elem == GetSnakeNumber())
		{
			elem = 0;
		}
		else if (elem == 2)
		{
			elem = 2;
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
	auto it = std::max_element(output.begin(), output.end());
	auto index = std::distance(output.begin(), it);
	if(index==0)
		return SnakeMove::FORWARD;
	if (index == 1)
		return SnakeMove::LEFT;
	if (index == 2)
		return SnakeMove::RIGHT;
	return SnakeMove::FORWARD;
}

std::vector<int> AI::Supervised::SupervisedBot::GetFieldOfView() const
{
	std::vector<int> field;
	int x = fieldX / 2;
	int y = fieldY / 2;
	int snakeX = snakeHead / cols;
	int snakeY = snakeHead % cols;
	int newX = snakeX - x;
	int newY = snakeY - y;
	for (auto i = 0; i < fieldX; ++i)
	{
		for (auto j = 0; j < fieldY; ++j)
		{
			auto val = (newX + i)*cols + (newY + j);
			if (val < 0 || val >= map.size())
				field.push_back(1);
			else
				field.push_back(map[val]);
		}
	}
	return field;
}
