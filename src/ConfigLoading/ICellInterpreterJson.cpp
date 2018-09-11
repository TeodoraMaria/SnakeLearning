#include "ICellInterpreterJson.h"
#include "GameLogic/CellInterpreter/Basic3CellInterpreter.hpp"
#include "GameLogic/CellInterpreter/WallFoodBody.hpp"
#include "GameLogic/CellInterpreter/WallFoodEnemy.hpp"

using json = nlohmann::json;

void GameLogic::CellInterpreter::to_json(nlohmann::json& j, const ICellInterpreter* cellInterpreter)
{
	if (dynamic_cast<const Basic3CellInterpreter*>(cellInterpreter))
		j = json{{ "type", "Basic3CellInterpreter" }};
	else if (dynamic_cast<const WallFoodBody*>(cellInterpreter))
		j = json{{ "type", "WallFoodBody" }};
	else if (dynamic_cast<const WallFoodEnemy*>(cellInterpreter))
		j = json{{ "type", "WallFoodEnemy" }};
	else
		throw "Unprocessed to_json for the given ICellInterpreter. Please implement me.";
}

void GameLogic::CellInterpreter::from_json(const nlohmann::json& j, ICellInterpreterPtr& cellInterpreter)
{
	const auto type = j.at("type").get<std::string>();
	if (type == "Basic3CellInterpreter")
		cellInterpreter = j.get<std::shared_ptr<Basic3CellInterpreter>>();
	else if (type == "WallFoodBody")
		cellInterpreter = j.get<std::shared_ptr<WallFoodBody>>();
	else if (type == "WallFoodEnemy")
		cellInterpreter = j.get<std::shared_ptr<WallFoodEnemy>>();
	else
		throw "Invalid CellInterpreter.";
}

void GameLogic::CellInterpreter::from_json(const nlohmann::json& j, std::shared_ptr<Basic3CellInterpreter>& cellInterpreter)
{
	cellInterpreter.reset(new Basic3CellInterpreter());
}

void GameLogic::CellInterpreter::from_json(const nlohmann::json& j, std::shared_ptr<WallFoodBody>& cellInterpreter)
{
	cellInterpreter.reset(new WallFoodBody());
}

void GameLogic::CellInterpreter::from_json(const nlohmann::json& j, std::shared_ptr<WallFoodEnemy>& cellInterpreter)
{
	cellInterpreter.reset(new WallFoodEnemy());
}
