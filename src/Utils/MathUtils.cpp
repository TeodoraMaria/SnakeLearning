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

double Utils::Math::randomDouble(double lwoerBound, double upperBound)
{
    std::uniform_real_distribution<double> unif(lwoerBound, upperBound);
    std::default_random_engine engine;
    return unif(engine);
}
