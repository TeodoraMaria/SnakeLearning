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

#include "ConfigLoading/GameOptionsJson.h"
#include "ConfigLoading/OpenGLRendererModelJson.h"
#include "ConfigLoading/GameJson.h"
#include "ConfigLoading/ICellInterpreterJson.h"

#include "ConfigLoading/IPlayerJson.h"
#include "ConfigLoading/QTabStudentJson.h"

#include "AI/QLearning/QTabStudent.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <assert.h>

using namespace GameLogic;
using namespace AI::QLearning;
using namespace GameLogic::CellInterpreter;
using namespace GymEnv::StateObserver;
using namespace GymEnv::Utils;

void ConfigLoaderSandbox();

using json = nlohmann::json;

int main(int nargs, char** args)
{
	srand(time(nullptr));

//  GeneticSingleSnake();
//	MultisnakeMain();
//	ConfigLoaderSandbox();
//  GeneticSingleSnake();
	
	MultisnakeTfMain();

	// Data generation for Teo
//	for (auto i = 0; i < 100; i++)
//	{
//		const auto jsonFile = "./aux_files/json_test/Game.json";
//		std::ifstream stream(jsonFile);
//
//		json j;
//		stream >> j;
//
//		std::cout << std::setw(2) << j << std::endl;
//
//		auto gmOptions = j.at("gameOptions").get<GameOptions>();
//		auto players = j.at("players").get<std::vector<IPlayerPtr>>();
//		gmOptions.gameplayLog = "aux_files/playLogs/play_log" + std::to_string(i) + ".teo";
//
//		std::ofstream logStream(gmOptions.gameplayLog);
//		if (!logStream.is_open())
//		{
//			throw "Failed to open file.";
//		}
//
//		logStream << "[]" << std::endl;
//		logStream.close();
//
//		auto game = Game(gmOptions, players);
//
//		game.InitGame();
//		game.Play(2000);
//	}
	return 0;
}
