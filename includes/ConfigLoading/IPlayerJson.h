#pragma once

#include "GameLogic/IPlayer.h"
#include "GameLogic/HumanPlayer.h"
#include "AI/QLearning/QTabStudent.hpp"
#include <json.hpp>

void to_json(nlohmann::json& j, const IPlayer* player);
void to_json(nlohmann::json& j, const HumanPlayer* player);

void from_json(const nlohmann::json& j, IPlayerPtr& player);
