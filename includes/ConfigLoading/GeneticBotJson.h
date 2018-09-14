#pragma once

#include "AI/GeneticAlgorithm/GeneticBot.h"
#include <json.hpp>

namespace AI
{
    namespace GeneticAlgorithm
    {
        void from_json(const nlohmann::json& j, std::shared_ptr<GeneticBot>& player);
    }
}