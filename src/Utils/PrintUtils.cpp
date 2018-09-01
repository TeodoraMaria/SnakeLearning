#include "PrintUtils.h"
#include <iomanip>
#include <iostream>

using namespace Utils::Print;

void Utils::Print::PrintTable(
	std::vector<std::vector<double>>& table,
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
