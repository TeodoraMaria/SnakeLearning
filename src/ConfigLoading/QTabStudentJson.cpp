#include "QTabStudentJson.h"
#include "IObserverJson.h"
#include "ICellInterpreterJson.h"
#include "AI/QLearning/TabularQJsonUtils.h"

void AI::QLearning::from_json(
	const nlohmann::json& j,
	std::shared_ptr<AI::QLearning::QTabStudent>& player)
{
	player.reset(new QTabStudent(
		j.at("observer").get<std::shared_ptr<GymEnv::StateObserver::IStateObserver>>(),
		[]() { return 0.0; },
		j.at("actionQulityCompareEps").get<double>()
	));
	
	if (j.find("noise") != j.end())
		player->SetNoise(j.at("noise").get<double>());
	
	// Brains.
	if (j.find("brainsFilePath") != j.end() && j.find("brains") != j.end())
		throw "Ambiguous: Both brains and brains file path are provided.";
	
	if (j.find("brainsFilePath") != j.end())
	{
		const auto brainsFilePath = j.at("brainsFilePath").get<std::string>();
		player->SetQTab(AI::QLearning::Utils::LoadQTable(brainsFilePath.c_str()));
	}
	else if (j.find("brains") != j.end())
	{
		player->SetQTab(j.at("brains").get<AI::QLearning::QTable>());
	}
	else
	{
		// Do nothing.
	}
}

