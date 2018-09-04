#pragma once

#include <vector>

namespace Utils { namespace Print
{
	void PrintTable(
		const std::vector<std::vector<double>>& table,
		size_t precision = 10,
		size_t width = 15,
		size_t indexWidth = 5);
	
	void PrintTable(
		const std::vector<std::vector<int>>& table,
		size_t width = 15,
		size_t indexWidth = 5);
}}
