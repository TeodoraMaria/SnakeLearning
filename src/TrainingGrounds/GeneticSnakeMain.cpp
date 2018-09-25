#include "AI/QLearning/TabularTrainer.hpp"
#include "AI/QLearning/TabularQJsonUtils.h"
#include "AI/HardCoded/SingleBot.hpp"
#include "AI/GeneticAlgorithm/GeneticTrainer.h"
#include "AI/GeneticAlgorithm/GeneticOptions.h"

#include "AI/GeneticAlgorithm/GeneticTrainerMultiSnake.h"

#include "GymEnv/SingleSnakeRelativeView.hpp"
#include "GymEnv/SingleSnakeGridView.hpp"
#include "GameView/OpenGLRenderer.h"

#include "GameLogic/CellInterpreter/Basic3CellInterpreter.hpp"
#include "GameLogic/CellInterpreter/WallFoodBody.hpp"

#include "ConfigLoading/GeneticBotJson.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <iomanip> 
#include <json.hpp>

using namespace AI::GeneticAlgorithm;
using namespace GameLogic;

void GeneticSnakeMain()
 {
    /*
     auto gmOptions = GameOptions();
     {
         gmOptions.boardLength = 25;
         gmOptions.boardWidth =25;
         gmOptions.numFoods = 10;
     }
     //auto gmRenderer = new GameView::TermRenderer();

     auto gmRenderer = new GameView::OpenGLRenderer(500, 500, gmOptions.boardLength, gmOptions.boardWidth);
     
     auto baseModel = GymEnv::SingleSnakeEnvBaseModel();

     baseModel.gmOptions = &gmOptions;
     baseModel.gmRenderer = gmRenderer;
     baseModel.celInterpreter = std::make_shared<CellInterpreter::WallFoodBody>();
     //baseModel.celInterpreter = std::make_shared<CellInterpreter::Basic3CellInterpreter>();
     
     auto gridModel = GymEnv::SingleSnakeGridViewModel();

     gridModel.gridHeight = 5;
     gridModel.gridWidth = 5;
     gridModel.deltaCoord = Coordinate(0, 0);
     gridModel.baseModel = baseModel;

    // auto env = new GymEnv::SingleSnakeRelativeView(baseModel);
     auto env = new GymEnv::SingleSnakeGridView(gridModel);

     AI::GeneticAlgorithm::GeneticOptions options;

     options.crossoverProb = 0.5;
     options.maxNumSteps = 100;
     options.mutationProb = 0.1;
     options.numEpisodes = 1000;
     options.numOfNetworks = 50;

     Utils::NetworkSettings settings;
     settings.m_inputs = env->GetObserver()->NbOfObservations() + 1;
     settings.m_hiddenLayersSizes = {4,6,8,4,3};

     auto trainer = AI::GeneticAlgorithm::GeneticTrainer(settings,options, env);
     GeneticBot& bot = dynamic_cast<GeneticBot&>(*trainer.Train());
     */

    GeneticTrainerMultiSnake s;
    GeneticBot& bot = dynamic_cast<GeneticBot&>(*s.Train());
    

     const auto filePath = "D:\\fac\\snake\\aux_files\\genetic\\TrainedGenetic.json";
     
     std::ofstream outFileStream(filePath);



     if (!outFileStream.is_open()) {
         std::cout << "Failed to open " << filePath << std::endl;
         return;
     }

     try {
         outFileStream << std::setw(2) << nlohmann::json(&bot);
         std::cout << "was written" << std::endl;
         outFileStream.close();
     } catch (...) {
         std::cout << "Failed to save player." << std::endl;
     }
 }
