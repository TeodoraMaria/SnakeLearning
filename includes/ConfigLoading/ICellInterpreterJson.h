#pragma once

#include "GameLogic/CellInterpreter/ICellInterpreter.hpp"
#include <json.hpp>

namespace GameLogic { namespace CellInterpreter
{
	class Basic3CellInterpreter;
	class WallFoodBody;
	class WallFoodEnemy;

	void from_json(const nlohmann::json& j, ICellInterpreterPtr& cellInterpreter);
	void from_json(const nlohmann::json& j, std::shared_ptr<Basic3CellInterpreter>& cellInterpreter);
	void from_json(const nlohmann::json& j, std::shared_ptr<WallFoodBody>& cellInterpreter);
	void from_json(const nlohmann::json& j, std::shared_ptr<WallFoodEnemy>& cellInterpreter);
}}
