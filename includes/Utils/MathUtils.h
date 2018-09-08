#pragma once

namespace Utils { namespace Math
{
	bool Approx(double a, double b, double eps = 0.00001);
	double Lerp(double a, double b, double scalar);
    double ruleOfThree(double value, double envMaxRange, double valueMaxRange);
    double randomDouble(double lwoerBound, double upperBound);

    template<typename T>
    T randomNumber(T lowerBound, T upperBound);
    
	int DoubleToBinaryInt(double value, double eps = 0.00001);
}}
