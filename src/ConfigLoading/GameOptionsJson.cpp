#include "GameOptionsJson.h"
#include "OpenGLRendererModelJson.h"
#include "GameView/OpenGLRenderer.h"
#include "GameView/TermRenderer.hpp"

void from_json(const nlohmann::json& j, GameOptions& gmOptions)
{
    gmOptions.boardLength = j.at("boardLength").get<size_t>();
    gmOptions.boardWidth = j.at("boardWidth").get<size_t>();
    gmOptions.numFoods = j.at("numFoods").get<size_t>();
    gmOptions.initialSnakeSize = j.at("initialSnakeSize").get<size_t>();
	
    if (j.find("renderer") != j.end())
    {
    	const auto rendererType = j["renderer"].at("type").get<std::string>();
		
    	if (rendererType == "OpenGLRenderer")
    	{
    		if (j["renderer"].find("model") == j["renderer"].end())
    			throw "A model is required for OpenGLRenderer.";

			const auto model = j["renderer"].at("model").get<GameView::OpenGLRendererModel>();
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
