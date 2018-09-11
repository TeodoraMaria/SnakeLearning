#include "GameOptionsJson.h"
#include <iostream>
#include <iomanip>

using json = nlohmann::json;

void _please_ignore_this_file__it_is_used_to_ignore_cmake_build_warnings_()
{
	GameOptions a;
	
	auto b = json(a);
	std::cout << std::setw(2) << b;
	
	GameOptions c = b;
}
