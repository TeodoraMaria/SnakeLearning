#pragma once

#include "GameLogic/CellInterpreter/ICellInterpreter.hpp"
#include "GameLogic/CellInterpreter/Basic3CellInterpreter.hpp"
//WallFoodBody
//WallFoodEnemy
#include <json.hpp>

namespace GameLogic { namespace CellInterpreter
{
	void from_json(const nlohmann::json& j, ICellInterpreterPtr& cellInterpreter);
	void from_json(const nlohmann::json& j, std::shared_ptr<Basic3CellInterpreter>& cellInterpreter);
}}
