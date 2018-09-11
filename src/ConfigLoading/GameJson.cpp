#include "GameJson.h"
#include "GameOptionsJson.h"
#include "IPlayerJson.h"

Game nlohmann::adl_serializer<Game>::from_json(const json& j)
{
	auto gmOptions = j.at("gameOptions").get<GameOptions>();
	auto players = j.at("players").get<std::vector<IPlayerPtr>>();
	return Game(gmOptions, players);
}
