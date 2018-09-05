#include "AI/QLearning/TabularTrainer.hpp"
#include "AI/HardCoded/SingleBot.hpp"

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
	}
	
//	auto gmRenderer = new GameView::TermRenderer();
	auto gmRenderer = new GameView::OpenGLRenderer(
		500, 500,
		gmOptions.boardLength, gmOptions.boardWidth);

	auto baseModel = GymEnv::SingleSnakeEnvBaseModel();
	{
		baseModel.gmOptions = &gmOptions;
		baseModel.gmRenderer = gmRenderer;
		baseModel.celInterpreter = std::make_shared<CellInterpreter::Basic3CellInterpreter>();
//		baseModel.celInterpreter = std::make_shared<CellInterpreter::WallFoodBody>();
	}
	
	auto env = new GymEnv::SingleSnakeRelativeView(baseModel);
	auto qoptions = AI::QLearning::QOptions();
	{
		qoptions.maxNumSteps = [](int episode) { return episode + 100; };
		qoptions.numEpisodes = 10000;
		qoptions.randActionDecayFactor = 1.0 / (qoptions.numEpisodes * 9);
		qoptions.learningRate = 0.1;
		qoptions.minRandActionChance = 0;
		qoptions.maxStepsWithoutFood = [&](int episode) -> size_t
		{
			return 150u + (double)episode / qoptions.numEpisodes * 300.0;
		};
		qoptions.milsToSleepBetweenFrames = 50;
	}
	
	auto trainer = AI::QLearning::TabularTrainer(qoptions, env);
	trainer.Train();
}

void MainSingleSnakeGridView()
{
	auto gmOptions = GameOptions();
	{
		gmOptions.boardLength = 25;
		gmOptions.boardWidth = 25;
		gmOptions.numFoods = 10;
	}
	
//	auto gmRenderer = new GameView::TermRenderer();
	auto gmRenderer = new GameView::OpenGLRenderer(
		500, 500,
		gmOptions.boardLength, gmOptions.boardWidth);
	auto baseModel = GymEnv::SingleSnakeEnvBaseModel();
	{
		baseModel.gmOptions = &gmOptions;
		baseModel.gmRenderer = gmRenderer;
//		baseModel.celInterpreter = std::make_shared<CellInterpreter::Basic3CellInterpreter>();
		baseModel.celInterpreter = std::make_shared<CellInterpreter::WallFoodBody>();
	}
	
	auto gridModel = GymEnv::SingleSnakeGridViewModel();
	{
		gridModel.baseModel = baseModel;
		gridModel.gridWidth = 2;
		gridModel.gridHeight = 7;
		gridModel.deltaCoord = Coordinate(0, 0);
	}
	
	auto env = new GymEnv::SingleSnakeGridView(gridModel);
	auto qoptions = AI::QLearning::QOptions();
	{
		qoptions.maxNumSteps = [&](int episode)
		{
			return 50 + (double)episode / qoptions.numEpisodes * 1000;
		};
		qoptions.numEpisodes = 50000;
		qoptions.randActionDecayFactor = 1.0 / (qoptions.numEpisodes * 9);
		qoptions.learningRate = 0.1;
		qoptions.minRandActionChance = 0;
		qoptions.maxStepsWithoutFood = [&](int episode) -> size_t
		{
			return 50u + (double)episode / qoptions.numEpisodes * 500.0;
		};
		
//		auto qInitDistrib = std::uniform_real_distribution<>(-1.0, 1.0);
		qoptions.tabInitializer = [&](std::mt19937& merseneTwister)
		{
//			return qInitDistrib(merseneTwister);
			return 0.5;
		};
		qoptions.milsToSleepBetweenFrames = 20;
	}
	
	auto trainer = AI::QLearning::TabularTrainer(qoptions, env);
	trainer.Train();
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

int main()
{
	srand(time(nullptr));
	
//	MainSingleSnakeRelativeView();
	MainSingleSnakeGridView();
	return 0;
}
