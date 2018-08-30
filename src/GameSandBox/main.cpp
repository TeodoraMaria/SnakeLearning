#include <iostream>
#include <iomanip>
#include <vector>
#include "GameLogic/Game.h"
#include "GameLogic/HumanPlayer.h"

int main()
{
	std::vector<HumanPlayer> players;
	players.push_back(HumanPlayer());
	players.push_back(HumanPlayer());
	GameOptions gameOptions(GameBoardType::BOX, 10, 20, players.size(),4);
	std::cout << "W for foward, A for left, D for right\n\n";
	Game game(gameOptions, players);
	srand(time(NULL));
	game.InitGame();
	game.Play();
	return 0;
}
