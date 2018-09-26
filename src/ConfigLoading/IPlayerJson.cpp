#include "IObserverJson.h"

#include "IPlayerJson.h"
#include "QTabStudentJson.h"
#include "GameLogic/HumanPlayer.h"
#include "ICellInterpreterJson.h"
#include "QTabStudentJson.h"
#include "GeneticBotJson.h"
#include <fstream>

using json = nlohmann::json;

struct FileNotFoundException : std::exception {};

void to_json(nlohmann::json& j, const IPlayer* player)
{
	if (dynamic_cast<const HumanPlayer*>(player))
		to_json(j, dynamic_cast<const HumanPlayer*>(player));
	else if (dynamic_cast<const AI::QLearning::QTabStudent*>(player))
		to_json(j, dynamic_cast<const AI::QLearning::QTabStudent*>(player));
    else if (dynamic_cast<const AI::GeneticAlgorithm::GeneticBot*>(player)) {
        to_json(j, dynamic_cast<const AI::GeneticAlgorithm::GeneticBot*>(player));
    } else {
        throw "Undefined to_json for the given IPlayer.";
    }
}

void to_json(nlohmann::json& j, const HumanPlayer* player)
{
	j = json{{ "type", "HumanPlayer" }};
}

static void LoadPlayerFromFile(const std::string filePath, IPlayerPtr& player)
{
	std::ifstream stream(filePath);

	if (!stream.is_open())
		throw FileNotFoundException();

	auto j = nlohmann::json::parse(stream);
	stream.close();
	
	player = j.get<IPlayerPtr>();
}

void from_json(const nlohmann::json& j, IPlayerPtr& player)
{
	const auto playerType = j.at("type").get<std::string>();
	
	if (playerType == "HumanPlayer")
		player.reset(new HumanPlayer());
	else if (playerType == "QTabStudent")
		player = j.get<std::shared_ptr<AI::QLearning::QTabStudent>>();
	else if (playerType == "LoadFromFile")
		LoadPlayerFromFile(j.at("filePath"), player);
	else
		throw "Invalid player type.";
}
