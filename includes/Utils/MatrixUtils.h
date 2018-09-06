#pragma once

#include <vector>
#include <functional>

namespace Utils { namespace Matrix
{
	typedef std::vector<std::vector<double>> DoubleMatrix;
	
	std::vector<double> MakeVector(
		size_t size,
		std::function<double ()> getInitValue);
	
	DoubleMatrix MakeMatrix(
		size_t lines,
		size_t cols,
		std::function<double ()> getInitValue);
}}
