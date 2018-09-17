#include "SupervisedBot.h"
#include "SupervisedTrainer.h"
#include <algorithm>

using namespace AI::Supervised;


std::vector<int> OneHotEncoder(int x)
{
	std::vector<int> label = { 0,0,0 };
	label[x] = 1;
	return label;
}

TrainingData GetTrainingData()
{
	std::ifstream file;
	int balance = 0;
	file.open("D:\\SnakeData.txt", std::fstream::in);
	TrainingSet ts;
	int lines = 0;
	while (!file.eof())
	{
		int val;
		lines++;
		TrainingEntry te;
		for (int i = 0; i < 49; i++)
		{
			file >> val;
			te.m_inputs.push_back(val);
		}
		file >> val;
		if (val == 0)
			balance++;
		else balance--;
		te.m_expectedOutputs = OneHotEncoder(val);
		if(val!=0 || balance<-3)
			ts.push_back(te);
	}
	file.close();
	int nr = ts.size() / 5;

	std::random_shuffle(ts.begin(), ts.end());
	TrainingData td;

	TrainingSet::const_iterator first = ts.begin();
	TrainingSet::const_iterator last = ts.begin() + nr;
	TrainingSet gen(first, last);
	td.m_generalizationSet = gen;

	first = ts.begin() + nr;
	last = ts.begin() + 2 * nr;
	TrainingSet val(first, last);
	td.m_validationSet = val;

	first = ts.begin() + 2 * nr;
	last = ts.end();
	TrainingSet train(first, last);
	td.m_trainingSet = train;

	return td;
}

AI::Supervised::SupervisedBot::SupervisedBot()
	: m_network(SupervisedNetwork::Settings{ 49, 150, 3 } )
{
	SupervisedTrainer::Settings trainerSettings;
	trainerSettings.m_learningRate = 0.0001;
	trainerSettings.m_momentum = 0.9;
	trainerSettings.m_useBatchLearning = true;
	trainerSettings.m_maxEpochs = 40000;
	trainerSettings.m_desiredAccuracy = 90;

	SupervisedTrainer trainer(trainerSettings, &m_network);

	TrainingData td = GetTrainingData();
	trainer.Train(td);
}


SnakeMove AI::Supervised::SupervisedBot::GetNextAction(const GameState & gameState)
{
	auto input = ExtractInput(gameState);
	auto output = m_network.Evaluate(input);
	return InterpretOutput(output);
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
