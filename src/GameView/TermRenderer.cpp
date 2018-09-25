#include "TermRenderer.hpp"
#include <iostream>
#include <assert.h>

#ifdef _WIN32
	#include <windows.h>
	#include <iomanip>
#endif


using namespace GameView;

static void PrinCell(const Coordinate& coord, const GameState& gameState)
{
	const auto cellValue = gameState.GetGameBoard()[coord];
	
	if (cellValue == BoardPart::EMPTY)
		std::cout << " ";
	else if (cellValue == BoardPart::WALL)
		std::cout << "\033[0;40m ";
	else if (cellValue == BoardPart::FOOD)
		std::cout << "\033[31m♥";
	else
	{
		const auto snakeIndex = cellValue - 10;
		assert(snakeIndex != 0);
		
		auto color = 0;
		if (snakeIndex <= 7)
			color = 40 + snakeIndex;
		else
			color = 100 + (snakeIndex % 7);
		
		auto bodyChar = 'o';
		if (gameState.IsSnakeHead(coord))
			bodyChar = 'X';
		
		printf("\033[%dm%c", color, bodyChar);
	}
	
	// Reset.
	std::cout << "\033[0m";
}

#ifdef _WIN32
	// Haven't compiled on windows.
	void TermRenderer::Render(const GameState& gameState) const
	{
		HANDLE  hConsole;
		int k;

		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		for (size_t i = 0; i < gameState.GetGameBoard().GetBoardWidth(); i++)
		{
			for (size_t j = 0; j<gameState.GetGameBoard().GetBoardLength(); j++)
			{
				const auto coord = Coordinate(i, j);
				k = gameState.IsSnakeHead(coord) ? 12 : 15;
				SetConsoleTextAttribute(hConsole, k);
				std::cout << std::setw(4) << gameState.GetGameBoard()[coord] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
#else
	void TermRenderer::Render(const GameState& gameState) const
	{
		const auto gameBoard = gameState.GetGameBoard();
		
		for (size_t i = 0; i < gameBoard.GetBoardWidth(); i++)
		{
			for (size_t j = 0; j < gameBoard.GetBoardLength(); j++)
				PrinCell(Coordinate(i, j), gameState);
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
#endif
