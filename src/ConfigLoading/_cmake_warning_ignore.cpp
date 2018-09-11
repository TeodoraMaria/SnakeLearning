#include "GameOptionsJson.h"
#include "OpenGLRendererModelJson.h"
#include "GameJson.h"
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
	
	std::cout << std::setw(2) << j;
	GameOptions test(j);
	
	// Ignore unused var.
	(void)test;
}

void _please_ignore_this_file__it_is_used_to_ignore_cmake_build_warnings_()
{
	TestGameOptionsJson();
}
