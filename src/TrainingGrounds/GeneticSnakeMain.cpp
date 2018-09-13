#include "AI/QLearning/TabularTrainer.hpp"
#include "AI/QLearning/TabularQJsonUtils.h"
#include "AI/HardCoded/SingleBot.hpp"
#include "AI/GeneticAlgorithm/GeneticTrainer.h"
#include "AI/GeneticAlgorithm/GeneticOptions.h"

#include "GymEnv/SingleSnakeRelativeView.hpp"
#include "GymEnv/SingleSnakeGridView.hpp"
#include "GameView/OpenGLRenderer.h"

#include "GameLogic/CellInterpreter/Basic3CellInterpreter.hpp"
#include "GameLogic/CellInterpreter/WallFoodBody.hpp"

#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace AI::GeneticAlgorithm;
using namespace GameLogic;

void GeneticSnakeMain()
 {
     auto gmOptions = GameOptions();
     {
         gmOptions.boardLength = 25;
         gmOptions.boardWidth = 25;
         gmOptions.numFoods = 10;
     }
     //auto gmRenderer = new GameView::TermRenderer();

     auto gmRenderer = new GameView::OpenGLRenderer(500, 500, gmOptions.boardLength, gmOptions.boardWidth);
     

     auto baseModel = GymEnv::SingleSnakeEnvBaseModel();

     baseModel.gmOptions = &gmOptions;
     baseModel.gmRenderer = gmRenderer;
     baseModel.celInterpreter = std::make_shared<CellInterpreter::WallFoodBody>();
     //baseModel.celInterpreter = std::make_shared<CellInterpreter::Basic3CellInterpreter>();
     


     auto env = new GymEnv::SingleSnakeRelativeView(baseModel);

     AI::GeneticAlgorithm::GeneticOptions options;

     options.crossoverProb = 0.4;
     options.maxNumSteps = 50;
     options.mutationProb = 0.04;
     options.numEpisodes = 1000;
     options.numOfNetworks = 50;

     Utils::NetworkSettings settings;
     settings.m_inputs = env->GetObserver()->NbOfObservations() + 1;
     settings.m_hiddenLayersSizes = {15,3};

     auto trainer = AI::GeneticAlgorithm::GeneticTrainer(settings,options, env);
     trainer.Train();

 }
