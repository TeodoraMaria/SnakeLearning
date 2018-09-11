#pragma once

#include "AI/QLearning/QTabStudent.hpp"
#include <json.hpp>

namespace AI { namespace QLearning
{
	void from_json(const nlohmann::json& j, std::shared_ptr<AI::QLearning::QTabStudent>& player);
}}
