#include <iostream>
#include <iomanip>
#include <vector>
#include <windows.h>  
#include "GameLogic/Game.h"
#include "GameLogic/HumanPlayer.h"



int main()
{
	HANDLE  hConsole;
	int k;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	std::vector<HumanPlayer> players;
	players.push_back(HumanPlayer());
	players.push_back(HumanPlayer());
	GameOptions gameOptions(GameBoardType::BOX, 10, players.size());
	std::cout << "0 for foward, 1 for left, 2 for right\n\n";
	Game game(gameOptions, players);
	srand(time(NULL));
	game.InitGame();
	game.Play();
	return 0;
}