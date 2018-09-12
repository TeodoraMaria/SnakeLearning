#pragma once

#include "GameView/OpenGLRenderer.h"
#include <json.hpp>

namespace GameView
{
	void from_json(const nlohmann::json& j, OpenGLRendererModel& glModel);
}
