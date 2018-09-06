#include <iostream>
#include <iomanip>
#include <vector>
#include "GameLogic/Game.h"
#include "GameLogic/HumanPlayer.h"
#include "GameView/OpenGLRenderer.h"
#include "AI/HardCoded/SingleBot.hpp"

int main(int argc, char** argv)
{
	srand(time(NULL));

	std::vector<IPlayerPtr> players(
	{
		IPlayerPtr(new HumanPlayer()),
		//IPlayerPtr(new AI::HardCoded::SingleBot()),
		//IPlayerPtr(new AI::HardCoded::SingleBot()),
		//IPlayerPtr(new AI::HardCoded::SingleBot()),
	});
	
	GameOptions gameOptions;
	{
		gameOptions.boardLength = 10;
		gameOptions.boardWidth = 10;
		gameOptions.numberOfPlayers = players.size();
		gameOptions.numFoods = 1;
		gameOptions.initialSnakeSize = 2;
		//gameOptions.playWithoutRenedring = true;
		gameOptions.saveGameplay = true;
		gameOptions.gameRenderer = new GameView::OpenGLRenderer(500,500,10,10);
	}
	Game game(gameOptions, players);
	
	std::cout << "W for foward, A for left, D for right\n\n";
	
	game.InitGame();
	game.Play();
	
	delete gameOptions.gameRenderer;
	return 0;
}
