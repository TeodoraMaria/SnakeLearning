#include "TrainingGrounds.h"

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
#include <iomanip>
#include <SDL.h>

using namespace GameLogic;

// void GeneticSingleSnake()
// {
//     auto gmOptions = GameOptions();
//     {
//         gmOptions.boardLength = 25;
//         gmOptions.boardWidth = 25;
//         gmOptions.numFoods = 10;
//     }
//     auto gmRenderer = new GameView::TermRenderer();
//
//     auto baseModel = GymEnv::SingleSnakeEnvBaseModel();
//
//     baseModel.gmOptions = &gmOptions;
//     baseModel.gmRenderer = gmRenderer;
//     baseModel.celInterpreter = std::make_shared<CellInterpreter::WallFoodBody>();
//
//     auto env = new GymEnv::SingleSnakeRelativeView(baseModel);
//
//     AI::GeneticAlgorithm::GeneticOptions options;
//
//     options.crossoverProb = 0.4;
//     options.maxNumSteps = 100;
//     options.mutationProb = 0.04;
//     options.numEpisodes = 1000;
//     options.numOfNetworks = 50;
//
//     auto trainer = AI::GeneticAlgorithm::GeneticTrainer(options, env);
//     trainer.Train();
//
// }

void ConfigLoaderSandbox();

int main(int nargs, char** args)
{
	srand(time(nullptr));

//	MultisnakeMain();
	ConfigLoaderSandbox();
//    GeneticSingleSnake();
	return 0;
}
