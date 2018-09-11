#pragma once

#include "GameLogic/IPlayer.h"
#include "GameLogic/HumanPlayer.h"
#include <json.hpp>

template <typename BasicJsonType>
void from_json(const BasicJsonType& j, IPlayerPtr& player)
{
	const auto playerType = j.at("type").template get<std::string>();
	
	if (playerType == "HumanPlayer")
		player.reset(new HumanPlayer());
	else
		throw "Invalid player type.";
}
