#include "QTabStudentJson.h"
#include "IObserverJson.h"
#include "ICellInterpreterJson.h"
#include "AI/QLearning/TabularQJsonUtils.h"

void AI::QLearning::from_json(
	const nlohmann::json& j,
	std::shared_ptr<AI::QLearning::QTabStudent>& player)
{
	auto a = j.at("cellInterpreter").get<GameLogic::CellInterpreter::ICellInterpreterPtr>();
	player.reset(new QTabStudent(
		j.at("cellInterpreter").get<GameLogic::CellInterpreter::ICellInterpreterPtr>(),
		j.at("observer").get<std::shared_ptr<GymEnv::StateObserver::IStateObserver>>(),
		[]() { return 0.0; },
		j.at("actionQulityCompareEps").get<double>()
	));
	
	const auto brainsFilePath = j.at("brains").get<std::string>();
	player->SetQTab(AI::QLearning::Utils::LoadQTable(brainsFilePath.c_str()));
}
