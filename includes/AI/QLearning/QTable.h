#pragma once

#include <vector>
#include <unordered_map>

namespace AI { namespace QLearning
{
	typedef std::unordered_map<int, std::vector<double>> QTable;
}}
