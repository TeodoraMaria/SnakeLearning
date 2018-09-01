#include "MatrixUtils.h"
#include "MathUtils.h"

using namespace Utils::Matrix;

DoubleMatrix Utils::Matrix::MakeMatrix(
	const size_t lines,
	const size_t cols,
	const double initValue)
{
	auto matrix = std::vector<std::vector<double>>(lines);
	for (auto i = 0u; i < matrix.size(); i++)
	{
		matrix[i] = std::vector<double>(cols);
		if (Utils::Math::Approx(initValue, 0))
			continue;
		
		for (auto& value : matrix[i])
			value = initValue;
	}
	
	return matrix;
}
