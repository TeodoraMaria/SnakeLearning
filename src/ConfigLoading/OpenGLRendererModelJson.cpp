#include "OpenGLRendererModelJson.h"

void GameView::from_json(const nlohmann::json& j, OpenGLRendererModel& glModel)
{
	glModel.resolutionX = j["resolutionX"].template get<size_t>();
	glModel.resolutionY = j["resolutionY"].template get<size_t>();
	glModel.lines = j["lines"].template get<size_t>();
	glModel.cols = j["cols"].template get<size_t>();
}
