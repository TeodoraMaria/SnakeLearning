#pragma once

#include <vector>

namespace Utils { namespace Print
{
	void PrintTable(
		std::vector<std::vector<double>>& table,
		size_t precision = 10,
		size_t width = 15,
		size_t indexWith = 5);
}}
