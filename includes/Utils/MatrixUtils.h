#pragma once

#include <vector>
#include <functional>

namespace Utils { namespace Matrix
{
	typedef std::vector<std::vector<double>> DoubleMatrix;
	
	DoubleMatrix MakeMatrix(
		size_t lines,
		size_t cols,
		std::function<double ()> getInitValue);
}}
