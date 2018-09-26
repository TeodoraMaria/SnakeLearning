#include "AI/Supervised/SupervisedManager.h"
#include "AI/Supervised/SupervisedTrainer.h"
#include "GameLogic/Snake.h"
#include "GameLogic/Coordinate.h"
#include "GameLogic/FileHelper.h"
#include <tuple>
#include <string>
#include <random>
#include <json.hpp>

AI::Supervised::SupervisedManager::SupervisedManager()
{
	m_bot = nullptr;
}

AI::Supervised::SupervisedManager::~SupervisedManager()
{
	m_bot = nullptr;
}

IPlayerPtr AI::Supervised::SupervisedManager::GetSupervisedBot(const int fieldX, const int fieldY, const TrainingWay trainingWay)
{
	if (m_bot == nullptr)
	{
		InitializeBot(fieldX, fieldY);
		std::string fileName = "SupervisedBot_";
		fileName = fileName + std::to_string(fieldX)+"x"+std::to_string(fieldY)+"_"+std::to_string(trainingWay);
		LoadSupervisedBot(fileName);
	}
	return IPlayerPtr(m_bot);
}

void AI::Supervised::SupervisedManager::TrainSupervisedBot(const std::string & inputFilePath, const int fieldX, const int fieldY, const TrainingWay trainingWay)
{
	InitializeBot(fieldX, fieldY);

	SupervisedTrainer::Settings trainerSettings;
	trainerSettings.m_learningRate = 0.0003;
	trainerSettings.m_momentum = 0.9;
	trainerSettings.m_useBatchLearning = false;
	trainerSettings.m_maxEpochs = 1000;
	trainerSettings.m_desiredAccuracy = 90;

	SupervisedTrainer trainer(trainerSettings, m_bot->GetNetwork());

	TrainingData td = GetTrainingData(fieldX, fieldY, inputFilePath, trainingWay);
	trainer.Train(td);
}

void AI::Supervised::SupervisedManager::LoadSupervisedBot(std::string fileName)
{
	//do stuff
}

void AI::Supervised::SupervisedManager::SaveSupervisedBot() const
{
}

TrainingData AI::Supervised::SupervisedManager::GetTrainingData(const int fieldX,const int fieldY , const std::string & inputFilePath, const TrainingWay trainingWay) const
{
	std::ifstream file;
	int balance = 0;

	file.open(inputFilePath, std::fstream::in);
	if (!file.is_open())
	{
		throw "File not found";
	}

	nlohmann::json j;
	file >> j;

	std::vector<GameplayStep> a = j.get < std::vector<GameplayStep>>();

	TrainingSet unbalancedts;
	int cols;
	std::vector<int> map;
	int snakeHead, snakeNeck;
	while (!file.eof())
	{
		int val;
		char aux;
		file >> cols >> aux;
		TrainingEntry te;
		for (int i = 0; i < 625; i++)
		{
			file >> val;
			map.push_back(val);
		}
		file >>aux>> snakeHead>>snakeNeck>>aux;
		std::vector<int> field = GetFieldOfView(map, snakeHead, snakeNeck, fieldX, fieldY, cols);
		std::for_each(field.begin(), field.end(), [&](auto& val) {
			if (trainingWay == TrainingWay::ENEMY)
				val = EnemyTranslate(val);
			else if (trainingWay == TrainingWay::FULL)
				val = FullTranslate(val, 11); //fix this, add snake number support
			else
				val = BasicTranslate(val);
			te.m_inputs.push_back(val);
		});
		file>>val;
		te.m_expectedOutputs = OneHotEncoder(val);
		//Normalize(te.m_inputs);
		unbalancedts.push_back(te);
	}
	file.close();
	std::vector<int> left = { 0,1,0 };
	std::vector<int> right = { 0,0,1 };
	auto lefts = std::count_if(unbalancedts.begin(), unbalancedts.end(), [left](const TrainingEntry& te) {return te.m_expectedOutputs == left;  });
	auto rights = std::count_if(unbalancedts.begin(), unbalancedts.end(), [right](const TrainingEntry& te) {return te.m_expectedOutputs == right;  });
	auto setNumber = lefts < rights ? lefts : rights;
	int nrl = setNumber, nrr = setNumber, nrf= setNumber;

	TrainingSet ts;
	std::for_each(unbalancedts.begin(), unbalancedts.end(), [&](const TrainingEntry& te) {
		if (te.m_expectedOutputs == left)
		{
			if(--nrl>=0)
				ts.push_back(te);
		}
		else if (te.m_expectedOutputs == right)
		{
			if(--nrr>=0)
				ts.push_back(te);
		}
		else { 
			if(--nrf>=0) 
				ts.push_back(te);
		}
	});

	std::random_shuffle(ts.begin(), ts.end());


	TrainingData td;
	int nr = ts.size() / 5;

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

std::vector<int> AI::Supervised::SupervisedManager::GetFieldOfView(const std::vector<int> map, const int snakeHead, const int snakeNeck, const int fieldX, const int fieldY, const int cols) const
{
	Snake snake(map[snakeHead]);
	Coordinate neck(snakeNeck / cols, snakeNeck%cols);
	Coordinate head(snakeHead / cols, snakeHead%cols);
	snake.Eat(neck);
	snake.Eat(head);
	GameBoard gb(map);
	GameState gs(gb, std::vector<Snake>());
	std::vector<int> field = gs.GetFieldOfView(snake, fieldX, fieldY);
	return field;
}

int AI::Supervised::SupervisedManager::BasicTranslate(int x) const
{
	if (x == 0)
		return 0;
	if (x == 2)
		return 1;
	return -1;
}

int AI::Supervised::SupervisedManager::EnemyTranslate(int x) const
{
	if (x == 0)
		return 1;
	if (x == 2)
		return 2;
	if (x == 1)
		return -1;
	return -2;
}

int AI::Supervised::SupervisedManager::FullTranslate(int x, int snakeNumber) const
{
	if (x == 0)
		return 1;
	if (x == 2)
		return 2;
	if (x == 1)
		return -1;
	if (x == snakeNumber)
		return 0;
	return -2;
}

void AI::Supervised::SupervisedManager::Normalize(std::vector<double>& inputs) const
{
	int min = *std::min_element(inputs.begin(), inputs.end());
	int diff = *std::max_element(inputs.begin(), inputs.end()) - min;
	std::for_each(inputs.begin(), inputs.end(), [&](double& x) {x = ((x - min) / diff); });
}

std::vector<int> AI::Supervised::SupervisedManager::OneHotEncoder(int x) const
{
	std::vector<int> label = { 0,0,0 };
	label[x] = 1;
	return label;
}

void AI::Supervised::SupervisedManager::InitializeBot(const int fieldX, const int fieldY)
{
	SupervisedNetwork::Settings settings;
	settings.m_numInputs = fieldX * fieldY;
	//settings.m_numInputs = 625; //temporary full map train
	settings.m_numHidden = 100;
	settings.m_numOutputs = 3;

	m_bot = new SupervisedBot(settings);
}
