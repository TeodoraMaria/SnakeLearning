#include "GameOptionsJson.h"
#include "OpenGLRendererModelJson.h"
#include "GameJson.h"
#include "ICellInterpreterJson.h"

#include "IPlayerJson.h"
#include "QTabStudentJson.h"

#include "AI/QLearning/QTabStudent.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <assert.h>

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

void TestQTabStudentFromFile()
{
	const auto jsonFile = "./aux_files/json_test/QTabStudent.json";
	std::ifstream stream(jsonFile);
	
	json j;
	stream >> j;
	
	std::cout << std::setw(2) << j << std::endl;
	
	auto agent = j.get<std::shared_ptr<AI::QLearning::QTabStudent>>();
	assert(agent.get() != nullptr);
	
	auto iAgent = j.get<IPlayerPtr>();
	assert(iAgent.get() != nullptr);
}

void TestQTabStudent()
{
	const auto jsonFile = "./aux_files/json_test/TrainedQTabAgent.json";
	std::ifstream stream(jsonFile);
	
	json j;
	stream >> j;
	
	auto agent = j.get<std::shared_ptr<AI::QLearning::QTabStudent>>();
	assert(agent.get() != nullptr);
	
	auto iAgent = j.get<IPlayerPtr>();
	assert(iAgent.get() != nullptr);
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

void ConfigLoaderSandbox()
{
//	TestGameOptionsJson();
//	TestGame();
//	TestQTabStudentFromFile();
	TestQTabStudent();
	
}
