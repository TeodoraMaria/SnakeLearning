#include "AI/QLearning/TabularTrainer.hpp"
#include "GameLogic/Game.h"
#include "GameLogic/HumanPlayer.h"
#include "AI/HardCoded/SingleBot.hpp"
#include <stdlib.h>
#include <time.h>
#include <iostream>

int main()
{
	srand(time(nullptr));
	
	auto trainer = AI::QLearning::TabularTrainer();
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
