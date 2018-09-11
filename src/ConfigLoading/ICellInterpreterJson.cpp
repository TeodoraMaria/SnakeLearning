#include "ICellInterpreterJson.h"

void GameLogic::CellInterpreter::from_json(
	const nlohmann::json& j,
	ICellInterpreterPtr& cellInterpreter)
{
	const auto type = j.at("type").get<std::string>();
	if (type == "Basic3CellInterpreter")
		cellInterpreter = j.get<std::shared_ptr<Basic3CellInterpreter>>();
	else
		throw "Invalid CellInterpreter.";
}

void GameLogic::CellInterpreter::from_json(
	const nlohmann::json& j,
	std::shared_ptr<Basic3CellInterpreter>& cellInterpreter)
{
	cellInterpreter.reset(new Basic3CellInterpreter());
}
