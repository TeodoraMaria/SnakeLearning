#pragma once

#include <vector>
#include <cstddef>

namespace Utils { namespace Print
{
	void PrintTable(
		const std::vector<std::vector<double>>& table,
		size_t precision = 10,
		size_t width = 15,
		size_t indexWidth = 5);
	
	void PrintTable(
		const std::vector<double>& table,
		size_t precision = 10,
		size_t width = 15);
	
	void PrintTable(
		const std::vector<std::vector<int>>& table,
		size_t width = 15,
		size_t indexWidth = 5);
	
	void PrintMatrix(
		const std::vector<double>& table,
		size_t matrixWidth,
		size_t precision = 2,
		size_t width = 4);
}}
