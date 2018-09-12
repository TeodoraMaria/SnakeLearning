#pragma once
#include "GameUtils.h"
#include "GameView/IGameRenderer.hpp"

/*
** If additional parameters are added to this model, please
** add them to CofigLoading/GameOptionsJson.h.
*/

struct GameOptions
{
	GameBoardType gameBoardType = GameBoardType::BOX;
	size_t boardLength = 10;
	size_t boardWidth = 10;
	size_t numFoods = 1;
	
	size_t initialSnakeSize = 3;

	GameView::IGameRenderer* gameRenderer = nullptr;
	bool saveGameplay = false;
};
