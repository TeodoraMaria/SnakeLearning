#include "SupervisedTrainer.h"
#include <iostream>
#include <algorithm>

using namespace AI::Supervised;
//
//std::vector<int> OneHotEncoder(int x)
//{
//	std::vector<int> label = { 0,0,0 };
//	label[x] = 1;
//	return label;
//}
//
//TrainingData GetTrainingData()
//{
//	std::ifstream file;
//	file.open("D:\\SnakeData.txt", std::fstream::in);
//	TrainingSet ts;
//	while (!file.eof())
//	{
//		int val;
//		TrainingEntry te;
//		for (int i = 0; i < 25; i++)
//		{
//			file >> val;
//			te.m_inputs.push_back(val/2);
//		}
//		file >> val;
//		te.m_expectedOutputs = OneHotEncoder(val);
//		ts.push_back(te);
//	}
//	file.close();
//	int nr = ts.size() / 5;
//	std::random_shuffle(ts.begin(), ts.end());
//	TrainingData td;
//
//	TrainingSet::const_iterator first = ts.begin();
//	TrainingSet::const_iterator last = ts.begin() + nr;
//	TrainingSet gen(first, last);
//	td.m_generalizationSet = gen;
//
//	first = ts.begin()+nr;
//	last = ts.begin() + 2*nr;
//	TrainingSet val(first, last);
//	td.m_validationSet = val;
//
//	first = ts.begin()+2*nr;
//	last = ts.end();
//	TrainingSet train(first, last);
//	td.m_trainingSet = train;
//
//	return td;
//}

int main()
{
	//// Create neural network
	//SupervisedNetwork::Settings networkSettings{ 25, 100, 3 };
	//SupervisedNetwork nn(networkSettings);
	//std::cout << "helooo";

	//// Create neural network trainer
	//SupervisedTrainer::Settings trainerSettings;
	//trainerSettings.m_learningRate = 0.0003;
	//trainerSettings.m_momentum = 0.9;
	//trainerSettings.m_useBatchLearning = true;
	//trainerSettings.m_maxEpochs = 2000;
	//trainerSettings.m_desiredAccuracy = 90;

	//SupervisedTrainer trainer(trainerSettings, &nn);

	//TrainingData td= GetTrainingData();
	//trainer.Train(td);

	return 0;
}