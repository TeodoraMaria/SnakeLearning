#include <iostream>
#include <iomanip>
#include <vector>
#include "GameLogic/Game.h"
#include "GameLogic/HumanPlayer.h"
#include "AI/HardCoded/SingleBot.hpp"

int main()
{
	srand(time(NULL));

	std::vector<IPlayerPtr> players(
	{
		IPlayerPtr(new HumanPlayer()),
//		IPlayerPtr(new AI::HardCoded::SingleBot()),
		//IPlayerPtr(new AI::HardCoded::SingleBot()),
		//IPlayerPtr(new AI::HardCoded::SingleBot()),
	});
	
	GameOptions gameOptions;
	{
		gameOptions.boardLength = 5;
		gameOptions.boardWidth = 5;
		gameOptions.numberOfPlayers = players.size();
		gameOptions.numFoods = 1;
		gameOptions.initialSnakeSize = 2;
	}
	Game game(gameOptions, players);
	
	std::cout << "W for foward, A for left, D for right\n\n";
	
	game.InitGame();
	game.Play();
	return 0;
}
