#include "GameOptionsJson.h"
#include "OpenGLRendererModelJson.h"
#include "GameJson.h"
#include "ICellInterpreterJson.h"

#include <iostream>
#include <iomanip>
#include <fstream>

using json = nlohmann::json;

void TestGameOptionsJson()
{
	const auto jsonFile = "./aux_files/json_test/GameOptions.json";
	std::ifstream stream(jsonFile);
	
	json j;
	stream >> j;
	
	std::cout << std::setw(2) << j << std::endl;
	GameOptions test(j);
	
	// Ignore unused var.
	(void)test;
}

void TestGame()
{
	const auto jsonFile = "./aux_files/json_test/Game.json";
	std::ifstream stream(jsonFile);

	json j;
	stream >> j;

	std::cout << std::setw(2) << j << std::endl;
	Game game(j);

	game.InitGame();
	game.Play();
}

void _please_ignore_this_file__it_is_used_to_ignore_cmake_build_warnings_()
{
//	TestGameOptionsJson();
	TestGame();
//	j.get<GameLogic::CellInterpreter::ICellInterpreter>();
}
