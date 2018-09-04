#include "AI/QLearning/TabularTrainer.hpp"
#include "AI/HardCoded/SingleBot.hpp"

#include "GymEnv/SingleSnakeRelativeView.hpp"
#include "GymEnv/SingleSnakeGridView.hpp"

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
		gmOptions.boardLength = 5;
		gmOptions.boardWidth = 5;
		gmOptions.numFoods = 3;
	}
	
	auto gmRenderer = new GameView::TermRenderer();
//	auto env = new GymEnv::SingleSnakeRelativeView(gmRenderer, gmOptions);
	
	const auto gridViewWidth = 3;
	const auto gridViewHeight = 3;
	auto env = new GymEnv::SingleSnakeGridView(
		gridViewWidth,
		gridViewHeight,
		gmRenderer,
		gmOptions);
	
	auto trainer = AI::QLearning::TabularTrainer(env);
	auto trainedAgent = trainer.Train();

//	std::vector<IPlayerPtr> players(
//	{
//		IPlayerPtr(trainedAgent),
//		IPlayerPtr(new AI::HardCoded::SingleBot()),
//	});
//
//	const GameOptions gameOptions(GameBoardType::BOX, 10, 20, players.size());
//	Game game(gameOptions, players);
//
//	game.InitGame();
//	game.Play();
	return 0;
}
