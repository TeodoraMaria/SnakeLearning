#pragma once

#include "GameLogic/GameOptions.h"
#include <json.hpp>

void from_json(const nlohmann::json& j, GameOptions& gmOptions);
