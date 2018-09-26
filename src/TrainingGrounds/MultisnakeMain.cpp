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
	auto trainer = AI::QLearning::MultisnakeTabularTrainer(true);
	
	auto callbacks = AI::ITrainer::TrainCallbacks();
	callbacks.numEpisodes = 10000;
	trainer.Train(callbacks);
}
