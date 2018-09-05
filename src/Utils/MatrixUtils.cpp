#include "MatrixUtils.h"
#include "MathUtils.h"

using namespace Utils::Matrix;

std::vector<double> Utils::Matrix::MakeVector(
	const size_t size,
	std::function<double ()> getInitValue)
{
	auto result = std::vector<double>(size);
	for (auto& value : result)
		value = getInitValue();
	
	return result;
}

DoubleMatrix Utils::Matrix::MakeMatrix(
	const size_t lines,
	const size_t cols,
	const std::function<double ()> getInitValue)
{
	auto matrix = std::vector<std::vector<double>>(lines);
	for (auto i = 0u; i < matrix.size(); i++)
	{
		matrix[i] = std::vector<double>(cols);
		for (auto& value : matrix[i])
			value = getInitValue();
	}
	
	return matrix;
}
