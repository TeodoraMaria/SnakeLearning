#include "GeneticJsonUtils.h"
#include "json.hpp"
#include <fstream>
#include <iomanip>

using json = nlohmann::json;
using namespace AI::GeneticAlgorithm;

void AI::GeneticAlgorithm::SaveWeights(const std::vector<float>& qtab, const char * filePath)
{
    std::ofstream fileStream;

    fileStream.open(filePath);
    if (!fileStream.is_open())
        throw "Failed to open file.";

    fileStream << std::setw(4) << json(qtab);
    fileStream.close();
}

std::vector<float> AI::GeneticAlgorithm::LoadWeights(const char * filePath)
{
    std::ifstream fileStream;

    fileStream.open(filePath);
    if (!fileStream.is_open())
        throw "Failed to open file.";

    json fileJsonContent;

    fileStream >> fileJsonContent;
    return fileJsonContent.get<std::vector<float>>();
}
