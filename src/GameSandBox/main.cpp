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
		//IPlayerPtr(new AI::HardCoded::SingleBot()),
		//IPlayerPtr(new AI::HardCoded::SingleBot()),
		//IPlayerPtr(new AI::HardCoded::SingleBot()),
	});
	
	const GameOptions gameOptions(GameBoardType::BOX, 6, 5, players.size(),3);
	Game game(gameOptions, players);
	
	std::cout << "W for foward, A for left, D for right\n\n";
	
	game.InitGame();
	game.Play();
	return 0;
}
