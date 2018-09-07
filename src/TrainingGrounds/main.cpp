#include "AI/QLearning/TabularTrainer.hpp"
#include "AI/QLearning/TabularQJsonUtils.h"
#include "AI/QLearning/TabularAgent.hpp"
#include "AI/QLearning/TabularActionPicker.hpp"
#include "AI/QLearning/MultiSnakeTabularTrainer.hpp"

#include "AI/HardCoded/SingleBot.hpp"


#include "AI/GeneticAlgorithm/GeneticTrainer.h"
#include "AI/GeneticAlgorithm/GeneticOptions.h"


#include "GymEnv/SingleSnakeRelativeView.hpp"
#include "GymEnv/SingleSnakeGridView.hpp"
#include "GymEnv/MultiSnakeEnv.hpp"
#include "GameView/OpenGLRenderer.h"

#include "GameLogic/CellInterpreter/Basic3CellInterpreter.hpp"
#include "GameLogic/CellInterpreter/WallFoodBody.hpp"
#include "GameLogic/Game.h"
#include "GameLogic/HumanPlayer.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace GameLogic;
using namespace AI::QLearning;

size_t g_numEpisodes = 0;
double g_randomDecayEpisodeMult = 1;


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
		gmOptions.boardLength = 20;
		gmOptions.boardWidth = 20;
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
			return 500 + (double)episode / qoptions.numEpisodes * 1000;
		};
		qoptions.qDiscountFactor = 0.8;
		qoptions.actionQualityEps = 0.005;
		
		if (g_numEpisodes != 0)
			qoptions.numEpisodes = g_numEpisodes;
		else
			qoptions.numEpisodes = 20000;

		qoptions.randActionDecayFactor =
			1.0 / (qoptions.numEpisodes * 5 * g_randomDecayEpisodeMult);
		qoptions.learningRate = 0.01;
		qoptions.minRandActionChance = 0;
		qoptions.maxStepsWithoutFood = [&](int episode) -> size_t
		{
			return 300u + (double)episode / qoptions.numEpisodes * 100.0;
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
		qoptions.printDieStates = false;
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

void MainMultiSnake()
{
	auto gmOptions = GameOptions();
	{
		gmOptions.boardLength = 25;
		gmOptions.boardWidth = 25;
		gmOptions.numFoods = 10;
		gmOptions.initialSnakeSize = 2;
		gmOptions.milsToWaitBetweenPrintFrames = 100;
//		gmOptions.gameRenderer = new GameView::TermRenderer();
		gmOptions.gameRenderer = new GameView::OpenGLRenderer(
			500, 500,
			gmOptions.boardLength, gmOptions.boardWidth);
	}
	
	auto qoptions = AI::QLearning::QOptions();
	{
		qoptions.maxNumSteps = [&](int episode)
		{
			return 500 + (double)episode / qoptions.numEpisodes * 1000;
		};
		qoptions.qDiscountFactor = 0.8;
		qoptions.actionQualityEps = 0.005;
		
		if (g_numEpisodes != 0)
			qoptions.numEpisodes = g_numEpisodes;
		else
			qoptions.numEpisodes = 20000;

		qoptions.randActionDecayFactor =
			1.0 / (qoptions.numEpisodes * 1 * g_randomDecayEpisodeMult);
		qoptions.learningRate = 0.01;
		qoptions.minRandActionChance = 0;
		qoptions.maxStepsWithoutFood = [&](int episode) -> size_t
		{
			return 300u + (double)episode / qoptions.numEpisodes * 100.0;
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
		qoptions.printDieStates = false;
	}
	
	auto merseneTwister = std::mt19937(std::random_device()());
	auto agents = std::vector<std::shared_ptr<TabularAgent>>();
	for (auto i = 0u; i < 2; i++)
	{
		auto qbrains = std::make_shared<QTabularBrains>(
			qoptions.learningRate,
			qoptions.qDiscountFactor,
			IPlayer::possibleMoves.size(),
			[&]() { return qoptions.tabInitializer(merseneTwister); });
		
		auto cellInterpreter = std::make_shared<CellInterpreter::Basic3CellInterpreter>();
		auto actionPicker = std::make_shared<TabularActionPicker>(
			cellInterpreter->NbOfInterpretableParts(),
			qoptions.actionQualityEps,
			qbrains);
		auto observer = std::make_shared<GymEnv::StateObserver::GridObserver>(cellInterpreter, 5, 5);
		
		agents.push_back(std::make_shared<TabularAgent>(qbrains, cellInterpreter, actionPicker, observer));
	}
	
	auto envModel = GymEnv::MultiSnakeEnvModel();
	{
		for (auto agent : agents)
			envModel.agents.push_back(std::static_pointer_cast<GymEnv::SnakeAgent>(agent));
		
		envModel.gameRenderer = gmOptions.gameRenderer;
		envModel.gmOptions = &gmOptions;
	}
	
	auto env = std::make_shared<GymEnv::MultiSnakeEnv>(envModel);
	auto trainer = MultiSnakeTabularTrainer(qoptions, agents, env);
	
	trainer.Train();
	
	auto players = std::vector<IPlayerPtr>();
	for (auto agent : agents)
			players.push_back(std::static_pointer_cast<IPlayer>(agent));
	
	Game game(gmOptions, players);

	for (auto i = 0; i < 50; i++)
	{
		game.InitGame();
		game.Play();
	}
}

void GeneticSingleSnake()
{
    auto gmOptions = GameOptions();
    {
        gmOptions.boardLength = 25;
        gmOptions.boardWidth = 25;
        gmOptions.numFoods = 10;
    }
    auto gmRenderer = new GameView::TermRenderer();

    auto baseModel = GymEnv::SingleSnakeEnvBaseModel();

    baseModel.gmOptions = &gmOptions;
    baseModel.gmRenderer = gmRenderer;
    baseModel.celInterpreter = std::make_shared<CellInterpreter::WallFoodBody>();

    auto env = new GymEnv::SingleSnakeRelativeView(baseModel);

    AI::GeneticAlgorithm::GeneticOptions options;

    options.crossoverProb = 0.4;
    options.maxNumSteps = 100;
    options.mutationProb = 0.04;
    options.numEpisodes = 1000;
    options.numOfNetworks = 50;

    auto trainer = AI::GeneticAlgorithm::GeneticTrainer(options, env);
    trainer.Train();

}

int main(int nargs, char** args)
{
	srand(time(nullptr));
	
	if (nargs > 1)
	{
		g_numEpisodes = std::atoi(args[1]);
	}
	
	if (nargs > 2)
	{
		g_randomDecayEpisodeMult = std::atof(args[2]);
	}
	
//	MainSingleSnakeRelativeView();
//	MainSingleSnakeGridView();
	MainMultiSnake();

//    GeneticSingleSnake();
	
	return 0;
}
