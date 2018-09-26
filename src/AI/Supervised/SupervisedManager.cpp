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

SupervisedBot* AI::Supervised::SupervisedManager::GetSupervisedBot(const int fieldX, const int fieldY, const TrainingWay trainingWay)
{
	if (m_bot == nullptr)
	{
		InitializeBot(fieldX, fieldY);
		std::string fileName = "SupervisedBot_";
		fileName = fileName + std::to_string(fieldX)+"x"+std::to_string(fieldY)+"_"+std::to_string(trainingWay)+".txt";
		LoadSupervisedBot(fileName);
	}
	return m_bot;
}

void AI::Supervised::SupervisedManager::TrainSupervisedBot(const std::string & inputFilePath, const int fieldX, const int fieldY, const TrainingWay trainingWay)
{
	//deprecated
}

IPlayerPtr AI::Supervised::SupervisedManager::Train(TrainCallbacks callbacks)
{
	int fieldX = 3, fieldY = 3, trainingWay = 0;
	std::string inputFilePath = "F:\\SnakeLearning\\aux_files\\playLogs\\play_log";
	InitializeBot(fieldX, fieldY);

	SupervisedTrainer::Settings trainerSettings;
	trainerSettings.m_learningRate = 0.0003;
	trainerSettings.m_momentum = 0.9;
	trainerSettings.m_useBatchLearning = false;
	trainerSettings.m_maxEpochs = callbacks.numEpisodes;
	trainerSettings.m_desiredAccuracy = 90;

	SupervisedTrainer trainer(trainerSettings, m_bot->GetNetwork());
	trainer.Reset();

	//setup();
	TrainingData td = GetTrainingData(fieldX, fieldY, inputFilePath, static_cast<TrainingWay>(trainingWay));

	for (size_t i = 0; i < callbacks.numEpisodes; i++) {
		trainer.RunEpoch(td.m_trainingSet);

		if (callbacks.emitDisplayGame)
		{
			callbacks.emitDisplayGame(IPlayerPtr(m_bot), 100);
		}

		if (callbacks.emitGraphValues)
		{
			double loss, acc;
			trainer.GetSetAccuracyAndMSE(td.m_generalizationSet, acc, loss);
			std::vector<double> values = { loss };

			callbacks.emitGraphValues(values);
		}

		if (callbacks.emitStepEpisode) {
			callbacks.emitStepEpisode(i + 1);
		}
	}
	std::string fileName = "SupervisedBot_";
	
	fileName = fileName + std::to_string(fieldX) + "x" + std::to_string(fieldY) + "_" + std::to_string(trainingWay) + ".txt";
	SaveSupervisedBot(fileName);
	return IPlayerPtr(m_bot);
}

void AI::Supervised::SupervisedManager::LoadSupervisedBot(std::string fileName)
{
	std::ifstream file(fileName);
	nlohmann::json j;

	file >> j;

	SupervisedNetwork net = j.get<SupervisedNetwork>();

	m_bot->SetNetwork(net);

	file.close();
}

void AI::Supervised::SupervisedManager::SaveSupervisedBot(std::string fileName) const
{
	std::ofstream file(fileName);
	file << nlohmann::json(*(m_bot->GetNetwork()));
	file.close();
}

TrainingData AI::Supervised::SupervisedManager::GetTrainingData(const int fieldX, const int fieldY , const std::string & inputFilePath, const TrainingWay trainingWay) const
{
	std::ifstream file;
	int balance = 0;
	int i = 0;
	nlohmann::json j;
	TrainingSet unbalancedts;
	std::string fileName = inputFilePath + std::to_string(i) + ".teo";
	file.open(fileName, std::fstream::in);
	while (file.is_open())
	{
		file >> j;

		std::vector<GameplayStep> steps = j.get < std::vector<GameplayStep>>();
		for (const auto& step : steps)
		{
			TrainingEntry te;
			std::vector<int> field = GetFieldOfView(step.view, step.snakeHeadPos, step.snakeNeckPos, fieldX, fieldY, step.boardLength);
			std::for_each(field.begin(), field.end(), [&](auto& val) {
				if (trainingWay == TrainingWay::ENEMY)
					val = EnemyTranslate(val);
				else if (trainingWay == TrainingWay::FULL)
					val = FullTranslate(val, 11); //fix this, add snake number support
				else
					val = BasicTranslate(val);
				te.m_inputs.push_back(val);
			});
			te.m_expectedOutputs = OneHotEncoder(static_cast<int>(step.move));
			//Normalize(te.m_inputs);
			unbalancedts.push_back(te);
		}

		file.close();
		++i;
		fileName = inputFilePath + std::to_string(i) + ".teo";
		file.open(fileName, std::fstream::in);
	}
	file.close();
	std::random_shuffle(unbalancedts.begin(), unbalancedts.end());
	/*std::vector<int> left = { 0,1,0 };
	std::vector<int> right = { 0,0,1 };
	auto lefts = std::count_if(unbalancedts.begin(), unbalancedts.end(), [left](const TrainingEntry& te) {return te.m_expectedOutputs == left;  });
	auto rights = std::count_if(unbalancedts.begin(), unbalancedts.end(), [right](const TrainingEntry& te) {return te.m_expectedOutputs == right;  });
	auto setNumber = lefts < rights ? lefts : rights;
	int nrl = setNumber, nrr = setNumber, nrf= setNumber;*/

	TrainingSet ts;
	for (auto& elem : unbalancedts)
	{
		ts.push_back(elem);
	}
	/*std::for_each(unbalancedts.begin(), unbalancedts.end(), [&](const TrainingEntry& te) {
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
	});*/

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
	GameBoard gb(map, cols);
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
