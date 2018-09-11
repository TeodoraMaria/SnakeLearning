#pragma once

#include "GameOptionsJson.h"
#include "IPlayerJson.h"
//#include "SmartPointerUtils.h"
#include "GameLogic/Game.h"
#include <json.hpp>

namespace nlohmann {
	template <>
    struct adl_serializer<Game> {
		
        static Game from_json(const json& j)
        {
			auto gmOptions = j.at("gameOptions").get<GameOptions>();
			auto players = j.at("players").get<std::vector<IPlayerPtr>>();
            return Game(gmOptions, players);
        }
    };
}
