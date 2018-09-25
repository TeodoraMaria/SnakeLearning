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

void MultisnakeMain()
{
	auto gmOptions = GameOptions();
	{
		gmOptions.boardLength = 25;
		gmOptions.boardWidth = 25;
		gmOptions.numFoods = 10;
		gmOptions.initialSnakeSize = 3;
		gmOptions.gameRenderer = new GameView::TermRenderer();
//		gmOptions.gameRenderer = new GameView::OpenGLRenderer(
//			500, 500,
//			gmOptions.boardLength, gmOptions.boardWidth);
	}
	
	auto qoptions = AI::QLearning::QOptions();
	{
		qoptions.maxNumSteps = [&](int episode)
		{
			return 50000;// + (double)episode / qoptions.numEpisodes * 3000;
		};
		qoptions.qDiscountFactor = 0.9;
		qoptions.actionQualityEps = 0.005;
		
		qoptions.numEpisodes = 10;
		qoptions.noiseDecayFactor = 1.0 / (qoptions.numEpisodes * 10);
		qoptions.learningRate = 0.1;
		qoptions.minNoise = 0.001;
		qoptions.maxStepsWithoutFood = [&](int episode) -> size_t
		{
			return 150u + (double)episode / qoptions.numEpisodes * 300.0;
		};
		
		qoptions.foodReward = [](int episode) { return 1.0; };
		qoptions.dieReward = [&](int episode) { return -1.0 + (double)episode / qoptions.numEpisodes * (-100.0); };
		qoptions.stepReward = [](int episode) { return 0; };
		
//		auto qInitDistrib = std::uniform_real_distribution<>(-1.0, 1.0);
		qoptions.tabInitializer = [&](std::mt19937& merseneTwister)
		{
//			return 0;
			return 0.5;
//			return qInitDistrib(merseneTwister);
		};
		qoptions.milsToSleepBetweenFrames = 25;
		qoptions.lastNGamesToRender = 5;
	}
	
	auto trainer = AI::QLearning::MultisnakeTabularTrainer(gmOptions, qoptions);
	trainer.Train();
}
