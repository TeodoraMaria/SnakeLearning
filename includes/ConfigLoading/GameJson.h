#pragma once

#include "GameLogic/Game.h"
#include <json.hpp>

namespace nlohmann
{
	template <>
    struct adl_serializer<Game>
    {
        static Game from_json(const json& j);
    };
}
