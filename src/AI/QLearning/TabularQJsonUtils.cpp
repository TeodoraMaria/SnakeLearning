#include "TabularQJsonUtils.h"
#include "json.hpp"
#include <fstream>
#include <iomanip>

using json = nlohmann::json;
using namespace AI::QLearning::Utils;

void AI::QLearning::Utils::SaveQTable(
	const QTable& qtab,
	const char* const filePath)
{
	std::ofstream fileStream;
	
	fileStream.open(filePath);
	if (!fileStream.is_open())
		throw "Failed to open file.";
	
	fileStream << std::setw(4) << json(qtab);
	fileStream.close();
}

AI::QLearning::QTable AI::QLearning::Utils::LoadQTable(const char* filePath)
{
	std::ifstream fileStream;
	
	fileStream.open(filePath);
	if (!fileStream.is_open())
		throw "Failed to open file.";
	
	json fileJsonContent;
	
	fileStream >> fileJsonContent;
	return fileJsonContent.get<QTable>();
}
