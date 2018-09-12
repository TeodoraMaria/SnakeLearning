#include "IObserverJson.h"
#include "ICellInterpreterJson.h"

using json = nlohmann::json;

void GymEnv::StateObserver::to_json(json& j, const IStateObserver* observer)
{
	if (dynamic_cast<const GridObserver*>(observer))
		to_json(j, dynamic_cast<const GridObserver*>(observer));
	else
		throw "to_json() for the given Observer type was not implemented.";
}

void GymEnv::StateObserver::to_json(json& j, const GridObserver* observer)
{
	j = {
		{ "type", "GridObserver" },
		{ "width", observer->GetWidth() },
		{ "height", observer->GetHeight() },
		{ "cellInterpreter", json(observer->GetCellInterpreter()) }
	};
}

void GymEnv::StateObserver::from_json(const json& j, std::shared_ptr<IStateObserver>& observer)
{
	const auto observerType = j.at("type").get<std::string>();
	
	if (observerType == "GridObserver")
		observer = j.get<std::shared_ptr<GridObserver>>();
	else
		throw "Invalid Observer type.";
}

void GymEnv::StateObserver::from_json(const json& j, std::shared_ptr<GridObserver>& observer)
{
	observer.reset(new GridObserver(
		j.at("cellInterpreter").get<GameLogic::CellInterpreter::ICellInterpreterPtr>(),
		j.at("width").get<size_t>(),
		j.at("height").get<size_t>()
	));
}
