#pragma once

#include <string>
#include <vector>

namespace AI
{
    namespace GeneticAlgorithm
    {

        void SaveWeights(const std::vector<float>& qtab, const char* filePath);
        std::vector<float> LoadWeights(const char* filePath);

    }
}
