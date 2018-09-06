#include "StateExtractor.hpp"
#include "Utils/PrintUtils.h"
#include "Utils/MathUtils.h"
#include <array>
#include <cmath>
#include <iostream>

using namespace GymEnv::Utils;

/*
** Receives a vector whcih contains values of 0 or 1.
*/

unsigned long long StateExtractor::BinaryVectorToNumber(
	const std::vector<double>& binTable,
	const size_t cellStates)
{
	unsigned long long result = 0;
	
	const auto numInterpretables = cellStates - 1;
	for (auto i = 0u; i < binTable.size() / numInterpretables; i++)
	{
		auto cellValue = 0;
		for (auto j = 0u; j < numInterpretables; j++)
		{
			const auto indx = i * numInterpretables + j;
			const auto intBinTableValue =
				::Utils::Math::DoubleToBinaryInt(binTable[indx]);
			
			if (intBinTableValue != 0)
			{
				cellValue = j + 1;
				break;
			}
		}
		
		result += cellValue * std::pow(cellStates, i);
	}
	return result;
}

