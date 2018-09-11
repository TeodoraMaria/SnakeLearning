#pragma once

#include "GameLogic/GameOptions.h"
#include <json.hpp>

template <typename BasicJsonType>
void to_json(BasicJsonType& j, const GameOptions& gmOptions)
{
	j = BasicJsonType{
		{ "boardLength", gmOptions.boardLength },
		{ "boardWidth", gmOptions.boardWidth },
		{ "numFoods", gmOptions.numFoods },
		{ "initialSnakeSize", gmOptions.initialSnakeSize }
	};
}

template <typename BasicJsonType>
void from_json(const BasicJsonType& j, GameOptions& gmOptions)
{
    gmOptions.boardLength = j["boardLength"].template get<size_t>();
    gmOptions.boardWidth = j["boardWidth"].template get<size_t>();
    gmOptions.numFoods = j["numFoods"].template get<size_t>();
    gmOptions.initialSnakeSize = j["initialSnakeSize"].template get<size_t>();
}
