#include "IPlayerJson.h"
#include "QTabStudentJson.h"
#include "GameLogic/HumanPlayer.h"

void from_json(const nlohmann::json& j, IPlayerPtr& player)
{
	const auto playerType = j.at("type").get<std::string>();
	
	if (playerType == "HumanPlayer")
		player.reset(new HumanPlayer());
	else
		throw "Invalid player type.";
}
