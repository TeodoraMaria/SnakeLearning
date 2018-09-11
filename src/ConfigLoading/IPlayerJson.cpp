#include "IPlayerJson.h"
#include "QTabStudentJson.h"
#include "GameLogic/HumanPlayer.h"
#include "ICellInterpreterJson.h"
#include "QTabStudentJson.h"

void from_json(const nlohmann::json& j, IPlayerPtr& player)
{
	const auto playerType = j.at("type").get<std::string>();
	
	if (playerType == "HumanPlayer")
		player.reset(new HumanPlayer());
	else if (playerType == "QTabStudent")
		player = j.get<std::shared_ptr<AI::QLearning::QTabStudent>>();
	else
		throw "Invalid player type.";
}
