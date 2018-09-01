#pragma once

#include <vector>

namespace Utils { namespace Matrix
{
	typedef std::vector<std::vector<double>> DoubleMatrix;
	
	DoubleMatrix MakeMatrix(size_t lines, size_t cols, double initValue = 0);
}}
