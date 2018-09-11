#include "IObserverJson.h"
#include "ICellInterpreterJson.h"

void GymEnv::StateObserver::from_json(
	const nlohmann::json& j,
	std::shared_ptr<IStateObserver>& observer)
{
	const auto observerType = j.at("type").get<std::string>();
	
	if (observerType == "GridObserver")
		observer = j.get<std::shared_ptr<GridObserver>>();
	else
		throw "Invalid Observer type.";
}

void GymEnv::StateObserver::from_json(
	const nlohmann::json& j,
	std::shared_ptr<GridObserver>& observer)
{
	observer.reset(new GridObserver(
		j.at("cellInterpreter").get<GameLogic::CellInterpreter::ICellInterpreterPtr>(),
		j.at("width").get<size_t>(),
		j.at("height").get<size_t>()
	));
}
