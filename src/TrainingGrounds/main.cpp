#include "AI/QLearning/TabularTrainer.hpp"
#include "AI/QLearning/TabularQJsonUtils.h"
#include "AI/HardCoded/SingleBot.hpp"
#include "AI/GeneticAlgorithm/GeneticTrainer.h"
#include "AI/GeneticAlgorithm/GeneticOptions.h"
#include "AI/QLearning/MultisnakeTabularTrainer.hpp"

#include "GymEnv/SingleSnakeRelativeView.hpp"
#include "GymEnv/SingleSnakeGridView.hpp"
#include "GameView/OpenGLRenderer.h"

#include "GameLogic/CellInterpreter/Basic3CellInterpreter.hpp"
#include "GameLogic/CellInterpreter/WallFoodBody.hpp"
#include "GameLogic/Game.h"
#include "GameLogic/HumanPlayer.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace GameLogic;

void MainSingleSnakeRelativeView()
{
	auto gmOptions = GameOptions();
	{
		gmOptions.boardLength = 25;
		gmOptions.boardWidth = 25;
		gmOptions.numFoods = 10;
		gmOptions.gameRenderer = new GameView::TermRenderer();
		// gmOptions.gameRenderer = new GameView::OpenGLRenderer(
		// 	500, 500,
		// 	gmOptions.boardLength, gmOptions.boardWidth);
	}
	
	auto baseModel = GymEnv::SingleSnakeEnvBaseModel();
	{
		baseModel.gmOptions = &gmOptions;
		baseModel.gmRenderer = gmOptions.gameRenderer;
		baseModel.celInterpreter = std::make_shared<CellInterpreter::Basic3CellInterpreter>();
//		baseModel.celInterpreter = std::make_shared<CellInterpreter::WallFoodBody>();
	}
	
	auto env = new GymEnv::SingleSnakeRelativeView(baseModel);
	auto qoptions = AI::QLearning::QOptions();
	{
		qoptions.maxNumSteps = [&](int episode)
		{
			return 50 + (double)episode / qoptions.numEpisodes * 1000;
		};
		qoptions.qDiscountFactor = 0.8;
		qoptions.actionQualityEps = 0.005;
		
		qoptions.numEpisodes = 10000;
		qoptions.randActionDecayFactor = 1.0 / (qoptions.numEpisodes / 2);
		qoptions.learningRate = 0.01;
		qoptions.minRandActionChance = 0;
		qoptions.maxStepsWithoutFood = [&](int episode) -> size_t
		{
			return 50u + (double)episode / qoptions.numEpisodes * 100.0;
		};
		
		qoptions.foodReward = [](int episode) { return 1.0; };
		qoptions.dieReward = [](int episode) { return 0; };
		qoptions.stepReward = [](int episode) { return 0; };
		
//		auto qInitDistrib = std::uniform_real_distribution<>(-1.0, 1.0);
		qoptions.tabInitializer = [&](std::mt19937& merseneTwister)
		{
			return 0;
//			return qInitDistrib(merseneTwister);
		};
		qoptions.milsToSleepBetweenFrames = 20;
	}
	
	auto trainer = AI::QLearning::TabularTrainer(qoptions, env);
	
	const auto jsonFilePath = std::string() +
		"aux_files/qtabular/RealtiveView.json";
	try
	{
		trainer.SetQTable(AI::QLearning::Utils::LoadQTable(jsonFilePath.c_str()));
	}
	catch (...)
	{
		std::cerr << "Warning: failed to load qtable." << std::endl;
	}
	
	trainer.Train();
	
	try
	{
		AI::QLearning::Utils::SaveQTable(trainer.GetQTable(), jsonFilePath.c_str());
	}
	catch (...)
	{
		std::cerr << "Warning: failed to save qtable." << std::endl;
	}
}

void MainSingleSnakeGridView()
{
	auto gmOptions = GameOptions();
	{
		gmOptions.boardLength = 25;
		gmOptions.boardWidth = 25;
		gmOptions.numFoods = 10;
		gmOptions.initialSnakeSize = 5;
//		gmOptions.gameRenderer = new GameView::TermRenderer();
		gmOptions.gameRenderer = new GameView::OpenGLRenderer(
			500, 500,
			gmOptions.boardLength, gmOptions.boardWidth);
	}
	
	auto baseModel = GymEnv::SingleSnakeEnvBaseModel();
	{
		baseModel.gmRenderer = gmOptions.gameRenderer;
		baseModel.gmOptions = &gmOptions;
//		baseModel.celInterpreter = std::make_shared<CellInterpreter::Basic3CellInterpreter>();
		baseModel.celInterpreter = std::make_shared<CellInterpreter::WallFoodBody>();
	}
	
	auto gridModel = GymEnv::SingleSnakeGridViewModel();
	{
		gridModel.baseModel = baseModel;
		gridModel.gridWidth = 5;
		gridModel.gridHeight = 5;
		gridModel.deltaCoord = Coordinate(0, 0);
	}
	
	auto env = new GymEnv::SingleSnakeGridView(gridModel);
	auto qoptions = AI::QLearning::QOptions();
	{
		qoptions.maxNumSteps = [&](int episode)
		{
			return 50 + (double)episode / qoptions.numEpisodes * 1000;
		};
		qoptions.qDiscountFactor = 0.8;
		qoptions.actionQualityEps = 0.005;
		
		qoptions.numEpisodes = 20000;
		qoptions.randActionDecayFactor = 1.0 / (qoptions.numEpisodes);
		qoptions.learningRate = 0.01;
		qoptions.minRandActionChance = 0;
		qoptions.maxStepsWithoutFood = [&](int episode) -> size_t
		{
			return 50u + (double)episode / qoptions.numEpisodes * 100.0;
		};
		
		qoptions.foodReward = [](int episode) { return 1.0; };
		qoptions.dieReward = [](int episode) { return 0; };
		qoptions.stepReward = [](int episode) { return -0.0001; };
		
//		auto qInitDistrib = std::uniform_real_distribution<>(-1.0, 1.0);
		qoptions.tabInitializer = [&](std::mt19937& merseneTwister)
		{
			return 0;
//			return qInitDistrib(merseneTwister);
		};
		qoptions.milsToSleepBetweenFrames = 10;
	}
	
	auto trainer = AI::QLearning::TabularTrainer(qoptions, env);
	
	const auto jsonFilePath = std::string() +
		"aux_files/qtabular/TrainedGrid" +
		std::to_string(gridModel.gridWidth) + "x" +
		std::to_string(gridModel.gridHeight) + ".json";
	
	try
	{
		trainer.SetQTable(AI::QLearning::Utils::LoadQTable(jsonFilePath.c_str()));
	}
	catch (...)
	{
		std::cerr << "Warning: failed to load qtable." << std::endl;
	}
	
	trainer.Train();
	
	try
	{
		AI::QLearning::Utils::SaveQTable(trainer.GetQTable(), jsonFilePath.c_str());
	}
	catch (...)
	{
		std::cerr << "Warning: failed to save qtable." << std::endl;
	}
//	auto trainedAgent = trainer.Train();
	
//
//		std::vector<IPlayerPtr> players(
//		{
//			IPlayerPtr(trainedAgent),
//	//		IPlayerPtr(new AI::HardCoded::SingleBot()),
//		});
//
//		Game game(gmOptions, players);
//
//		game.InitGame();
//		game.Play();
}

// void GeneticSingleSnake()
// {
//     auto gmOptions = GameOptions();
//     {
//         gmOptions.boardLength = 25;
//         gmOptions.boardWidth = 25;
//         gmOptions.numFoods = 10;
//     }
//     auto gmRenderer = new GameView::TermRenderer();

//     auto baseModel = GymEnv::SingleSnakeEnvBaseModel();

//     baseModel.gmOptions = &gmOptions;
//     baseModel.gmRenderer = gmRenderer;
//     baseModel.celInterpreter = std::make_shared<CellInterpreter::WallFoodBody>();

//     auto env = new GymEnv::SingleSnakeRelativeView(baseModel);

//     AI::GeneticAlgorithm::GeneticOptions options;

//     options.crossoverProb = 0.4;
//     options.maxNumSteps = 100;
//     options.mutationProb = 0.04;
//     options.numEpisodes = 1000;
//     options.numOfNetworks = 50;

//     auto trainer = AI::GeneticAlgorithm::GeneticTrainer(options, env);
//     trainer.Train();

// }

void MultisnakeMain()
{
	auto gmOptions = GameOptions();
	{
		gmOptions.boardLength = 25;
		gmOptions.boardWidth = 25;
		gmOptions.numFoods = 10;
		gmOptions.initialSnakeSize = 3;
//		gmOptions.gameRenderer = new GameView::TermRenderer();
		gmOptions.gameRenderer = new GameView::OpenGLRenderer(
			500, 500,
			gmOptions.boardLength, gmOptions.boardWidth);
	}
	
	auto baseModel = GymEnv::SingleSnakeEnvBaseModel();
	{
		baseModel.gmRenderer = gmOptions.gameRenderer;
		baseModel.gmOptions = &gmOptions;
		baseModel.celInterpreter = std::make_shared<CellInterpreter::Basic3CellInterpreter>();
//		baseModel.celInterpreter = std::make_shared<CellInterpreter::WallFoodBody>();
	}
	
	auto qoptions = AI::QLearning::QOptions();
	{
		qoptions.maxNumSteps = [&](int episode)
		{
			return 50000;// + (double)episode / qoptions.numEpisodes * 3000;
		};
		qoptions.qDiscountFactor = 0.8;
		qoptions.actionQualityEps = 0.01;
		
		qoptions.numEpisodes = 15000;
		qoptions.randActionDecayFactor = 1.0 / (qoptions.numEpisodes);
		qoptions.learningRate = 0.01;
		qoptions.minRandActionChance = 0;
		qoptions.maxStepsWithoutFood = [&](int episode) -> size_t
		{
			return 100u + (double)episode / qoptions.numEpisodes * 300.0;
		};
		
		qoptions.foodReward = [](int episode) { return 1.0; };
		qoptions.dieReward = [](int episode) { return 0; };
		qoptions.stepReward = [](int episode) { return 0; };
		
//		auto qInitDistrib = std::uniform_real_distribution<>(-1.0, 1.0);
		qoptions.tabInitializer = [&](std::mt19937& merseneTwister)
		{
			return 0;
//			return qInitDistrib(merseneTwister);
		};
		qoptions.milsToSleepBetweenFrames = 10;
	}
	
	auto trainer = AI::QLearning::MultisnakeTabularTrainer(gmOptions, qoptions);
	trainer.Train();
}

int main(int nargs, char** args)
{
	srand(time(nullptr));
	
//	MainSingleSnakeRelativeView();
//	MainSingleSnakeGridView();
	MultisnakeMain();

//    GeneticSingleSnake();
	
	return 0;
}
