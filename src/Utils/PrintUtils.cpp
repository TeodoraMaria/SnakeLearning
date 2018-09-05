#include "PrintUtils.h"
#include <iomanip>
#include <iostream>

using namespace Utils::Print;

void Utils::Print::PrintTable(
	const std::vector<std::vector<double>>& table,
	const size_t precision,
	const size_t width,
	const size_t indexWith)
{
	for (auto i = 0u; i < table.size(); i++)
	{
		std::cout << std::setw(indexWith) << i << ") ";
		for (auto j = 0u; j < table[i].size(); j++)
		{
			std::cout
				<< std::setw(width) << std::fixed
				<< std::setprecision(precision) << table[i][j];
		}
		std::cout << std::endl;
	}
}

void Utils::Print::PrintTable(
	const std::vector<double>& table,
	const size_t precision,
	const size_t width)
{
	for (const auto& value : table)
	{
		std::cout
			<< std::setw(width) << std::fixed
			<< std::setprecision(precision) << value;
	}
	std::cout << std::endl;
}

void Utils::Print::PrintTable(
	const std::vector<std::vector<int>>& table,
	const size_t width,
	const size_t indexWidth)
{
	for (auto i = 0u; i < table.size(); i++)
	{
		std::cout << std::setw(indexWidth) << i << ") ";
		for (auto j = 0u; j < table[i].size(); j++)
		{
			std::cout << std::setw(width) << table[i][j];
		}
		std::cout << std::endl;
	}
}
