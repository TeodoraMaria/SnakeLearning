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
	
	auto gmRenderer = new GameView::TermRenderer();
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
	}
	
	auto trainer = AI::QLearning::TabularTrainer(qoptions, env);
	trainer.Train();
}

int main()
{
	srand(time(nullptr));
	
	MainSingleSnakeRelativeView();
	return 0;
}
