#pragma once

#include "GameView/OpenGLRenderer.h"
#include <json.hpp>

namespace GameView
{
	template <typename BasicJsonType>
	void from_json(const BasicJsonType& j, OpenGLRendererModel& glModel)
	{
		glModel.resolutionX = j["resolutionX"].template get<size_t>();
		glModel.resolutionY = j["resolutionY"].template get<size_t>();
		glModel.lines = j["lines"].template get<size_t>();
		glModel.cols = j["cols"].template get<size_t>();
	}
}
