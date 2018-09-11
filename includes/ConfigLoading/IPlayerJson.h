#pragma once

#include "GameLogic/IPlayer.h"
#include <json.hpp>

void from_json(const nlohmann::json& j, IPlayerPtr& player);


