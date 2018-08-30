#pragma once

#include <string>

namespace MultiPlatform
{
	enum class Color
	{
		RED
	};
	
	void PrintColoredChar(Color color, char c);
	void PrintColoredStr(Color color, std::string str);
	
	int getch();
}
