#include <iostream>
#include <iomanip>
#include "GameLogic/Game.h"

int main()
{
	Game game(false,10,3,3);
	srand(time(NULL));
	game.InitGame();
	auto board = game.GetGameBoard();

	for (auto line : board)
	{
		for (auto elem : line)
		{
			std::cout << std::setw(4) << elem << " ";
		}
		std::cout << std::endl;
	}
	return 0;
}