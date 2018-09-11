#pragma once

#include "OpenGLRendererModelJson.h"
#include "GameLogic/GameOptions.h"
#include "GameView/OpenGLRenderer.h"
#include "GameView/TermRenderer.hpp"
#include <json.hpp>

template <typename BasicJsonType>
void from_json(const BasicJsonType& j, GameOptions& gmOptions)
{
    gmOptions.boardLength = j.at("boardLength").template get<size_t>();
    gmOptions.boardWidth = j.at("boardWidth").template get<size_t>();
    gmOptions.numFoods = j.at("numFoods").template get<size_t>();
    gmOptions.initialSnakeSize = j.at("initialSnakeSize").template get<size_t>();
	
    if (j.find("renderer") != j.end())
    {
    	const auto rendererType = j["renderer"].at("type").template get<std::string>();
		
    	if (rendererType == "OpenGLRenderer")
    	{
    		if (j["renderer"].find("model") == j["renderer"].end())
    			throw "A model is required for OpenGLRenderer.";

			const auto model = j["renderer"].at("model").template get<GameView::OpenGLRendererModel>();
			gmOptions.gameRenderer = new GameView::OpenGLRenderer(model);
		}
		else if (rendererType == "TermRenderer")
		{
			gmOptions.gameRenderer = new GameView::TermRenderer();
		}
		else
			throw "Invalid renderer type.";
	}
}
