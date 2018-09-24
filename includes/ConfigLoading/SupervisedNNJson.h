#pragma once

#include "AI/Supervised/SupervisedNetwork.h"
#include <json.hpp>

void to_json(nlohmann::json& j, const AI::Supervised::SupervisedNetwork* net);

void from_json(const nlohmann::json& j, AI::Supervised::SupervisedNetwork& net);