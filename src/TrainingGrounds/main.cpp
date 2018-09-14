#include "TrainingGrounds.h"

#include "AI/QLearning/TabularTrainer.hpp"
#include "AI/QLearning/TabularQJsonUtils.h"
#include "AI/HardCoded/SingleBot.hpp"
#include "AI/GeneticAlgorithm/GeneticTrainer.h"
#include "AI/GeneticAlgorithm/GeneticOptions.h"
#include "AI/QLearning/MultisnakeTabularTrainer.hpp"

#include "GymEnv/StateObserver/GridObserver.hpp"
#include "GymEnv/StateObserver/FoodDirectionDecorator.hpp"
#include "GymEnv/StateExtractor.hpp"
#include "GymEnv/SingleSnakeRelativeView.hpp"
#include "GymEnv/SingleSnakeGridView.hpp"
#include "GameView/OpenGLRenderer.h"

#include "GameLogic/CellInterpreter/WallFoodEnemy.hpp"
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
using namespace AI::QLearning;
using namespace GameLogic::CellInterpreter;
using namespace GymEnv::StateObserver;
using namespace GymEnv::Utils;

void ConfigLoaderSandbox();

int main(int nargs, char** args)
{
	srand(time(nullptr));

	MultisnakeMain();
//	ConfigLoaderSandbox();
//    GeneticSingleSnake();
	
	return 0;
}
