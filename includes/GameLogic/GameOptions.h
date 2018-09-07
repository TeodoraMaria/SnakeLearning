#pragma once
#include "GameUtils.h"
#include "GameView/IGameRenderer.hpp"

struct GameOptions
{
	GameBoardType gameBoardType = GameBoardType::BOX;
	size_t boardLength = 10;
	size_t boardWidth = 10;
	size_t numberOfPlayers = 1;
	size_t numFoods = 1;
	
	size_t initialSnakeSize = 3;

	GameView::IGameRenderer* gameRenderer = nullptr;

	bool playWithoutRenedring = false;
	bool saveGameplay = false;
	
	size_t milsToWaitBetweenPrintFrames = 0;
};
