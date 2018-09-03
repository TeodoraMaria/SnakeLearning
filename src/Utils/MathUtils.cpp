#include "MathUtils.h"
#include <cmath>

using namespace Utils::Math;

bool Utils::Math::Approx(const double a, const double b, const double eps)
{
	return std::abs(a - b) <= eps;
}

double Utils::Math::Lerp(const double a, const double b, const double scalar)
{
	return a + (b - a) * scalar;
}
