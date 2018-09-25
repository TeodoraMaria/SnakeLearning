#include "MathUtils.h"
#include <cmath>
#include <random>

using namespace Utils::Math;

bool Utils::Math::Approx(const double a, const double b, const double eps)
{
	return std::abs(a - b) <= eps;
}

double Utils::Math::Lerp(const double a, const double b, const double scalar)
{
	return a + (b - a) * scalar;
}

double Utils::Math::ruleOfThree(double value, double envMaxRange, double valueMaxRange)
{
    return value*envMaxRange / valueMaxRange;
}

double Utils::Math::randomDouble(double lowerBound, double upperBound)
{
    double f = (double)rand() / RAND_MAX;
    return lowerBound + f * (upperBound - lowerBound);
}

double Utils::Math::normalize(double x, double minimum, double maximum, double floor, double ceil)
{
    return ((ceil - floor) * (x - minimum)) / (maximum - minimum) + floor;
}



/*
** Convert the given value to a binary int by approximating.
*/

int Utils::Math::DoubleToBinaryInt(
	const double value,
	const double eps)
{
	if (Math::Approx(value, 0, eps))
		return 0;
	return 1;
}
