#include "TermRenderer.hpp"
#include <iostream>
#include <assert.h>

#ifdef _WIN32
	#include <windows.h>
	#include <assert.h>
#endif


using namespace GameView;

static void PrinCell(const int cellValue)
{
	if (cellValue == 0)
		std::cout << " ";
	else if (cellValue == -1)
		std::cout << "\033[0;40m ";
	else if (cellValue == 1)
		std::cout << "\033[5;31m♥";
	else
	{
		const auto snakeIndex = cellValue - 10;
		assert(snakeIndex != 0);
		
		auto color = 0;
		if (snakeIndex <= 7)
			color = 40 + snakeIndex;
		else
			color = 100 + (snakeIndex % 7);
		
		printf("\033[%dmo", color);
	}
	
	// Reset.
	std::cout << "\033[0m";
}

#ifdef _WIN32
	void TermRenderer::Render(const GameState& gameState) const
	{
		std::cout
			<< "PLease implement me."
			<< "Can't access IsSnakeHead() btw." << std::endl;
	}
#else
	void TermRenderer::Render(const GameState& gameState) const
	{
		const auto gameBoard = gameState.GetGameBoard();
		
		for (size_t i = 0; i < gameBoard.GetBoardWidth(); i++)
		{
			for (size_t j = 0; j < gameBoard.GetBoardLength(); j++)
				PrinCell(gameBoard[Coordinate(i, j)]);
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
#endif
