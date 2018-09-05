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

int main()
{
	srand(time(nullptr));
	
	auto gmOptions = GameOptions();
	{
		gmOptions.boardLength = 25;
		gmOptions.boardWidth = 25;
		gmOptions.numFoods = 10;
	}
	
	auto gmRenderer = new GameView::TermRenderer();
//	auto gmRenderer = new GameView::OpenGLRenderer(200, 200, gmOptions.boardLength, gmOptions.boardWidth);
	auto env = new GymEnv::SingleSnakeRelativeView(
		gmRenderer,
		gmOptions,
//		std::make_shared<GameLogic::CellInterpreter::Basic3CellInterpreter>()
		std::make_shared<GameLogic::CellInterpreter::WallFoodBody>()
	);
	
//	const auto gridViewWidth = 5;
//	const auto gridViewHeight = 3;
//	auto env = new GymEnv::SingleSnakeGridView(
//		gridViewWidth,
//		gridViewHeight,
//		gmRenderer,
//		gmOptions);
//
	auto qoptions = AI::QLearning::QOptions();
	{
		qoptions.maxNumSteps = [](int episode) { return episode + 100; };
		qoptions.numEpisodes = 40000;
		qoptions.randActionDecayFactor = 1.0 / (qoptions.numEpisodes * 9);
		qoptions.learningRate = 0.1;
		qoptions.minRandActionChance = 0;
		qoptions.maxStepsWithoutFood = [&](int episode) -> size_t
		{
			return 150u + (double)episode / qoptions.numEpisodes * 300.0;
		};
	}
	
	auto trainer = AI::QLearning::TabularTrainer(qoptions, env);
	auto trainedAgent = trainer.Train();

//	auto a = std::vector<int>({ 1, 0, 0, 0, 0, 0 });
//	std::cout << GymEnv::Utils::StateExtractor::BinaryVectorToNumber(a) << std::endl;
	
//	std::vector<IPlayerPtr> players(
//	{
//		IPlayerPtr(trainedAgent),
////		IPlayerPtr(new AI::HardCoded::SingleBot()),
//	});
//
//	Game game(gmOptions, players);
//
//	game.InitGame();
//	game.Play();
	return 0;
}
