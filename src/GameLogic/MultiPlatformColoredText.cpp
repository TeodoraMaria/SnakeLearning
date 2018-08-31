#include "MultiPlatformTerm.hpp"
#include <iostream>

#ifdef _WIN32
	#include <windows.h>
	#include <conio.h>
#else
	#include <unistd.h>
	#include <termios.h>
#endif

#ifndef _WIN32
	int getch()
	{
		int buf = 0;
		struct termios old = { 0, 0};
	
		if(tcgetattr(0, &old) < 0)
			perror("tcsetattr()");
	
		old.c_lflag &= ~ICANON;
		old.c_lflag &= ~ECHO;
		old.c_cc[VMIN] = 1;
		old.c_cc[VTIME] = 0;
	
		if(tcsetattr(0, TCSANOW, &old) < 0)
			perror("tcsetattr ICANON");
	
		if(read(0, &buf, 1) < 0)
			perror("read()");
	
		old.c_lflag |= ICANON;
		old.c_lflag |= ECHO;
	
		if(tcsetattr(0, TCSADRAIN, &old) < 0)
			perror("tcsetattr ~ICANON");
	
		return buf;
	 }
#endif

using namespace MultiPlatform;

#ifdef _WIN32
	void PrintColoredCharWindows(Color color, const char c)
	{
		HANDLE hConsole;
		int colorInt = 15;

		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		
		switch (color)
		{
			case MultiPlatform::Color::RED:
				colorInt = 12;
				break;
		}
		
		SetConsoleTextAttribute(hConsole, colorInt);
		std::cout << c;
	}
#else
	void PrintColoredCharNoWindows(Color color, const char c)
	{
		const char* const ansiRsest = "\033[0m";
		const char* ansiColor = ansiRsest;
		
		switch (color)
		{
			case Color::RED:
				ansiColor = "\033[31m";
				break;
		}
		
		std::cout << ansiColor << c << ansiRsest;
	}
#endif

void MultiPlatform::PrintColoredChar(const Color color, const char c)
{
	#ifdef _WIN32
		PrintColoredCharWindows(color, c);
	#else
		PrintColoredCharNoWindows(color, c);
	#endif
}

void MultiPlatform::PrintColoredStr(const Color color, const std::string str)
{
	for (auto i = 0; i < str.length(); i++)
	{
		PrintColoredChar(color, str[i]);
	}
}

int MultiPlatform::getch()
{
	#ifdef _WIN32
		
		const auto result = ::getch();
		::getch();
		return result;
	#else
		int buf = 0;
		struct termios old = { 0 };
	
		if(tcgetattr(0, &old) < 0)
			perror("tcsetattr()");
	
		old.c_lflag &= ~ICANON;
		old.c_lflag &= ~ECHO;
		old.c_cc[VMIN] = 1;
		old.c_cc[VTIME] = 0;
	
		if(tcsetattr(0, TCSANOW, &old) < 0)
			perror("tcsetattr ICANON");
	
		if(read(0, &buf, 1) < 0)
			perror("read()");
	
		old.c_lflag |= ICANON;
		old.c_lflag |= ECHO;
	
		if(tcsetattr(0, TCSADRAIN, &old) < 0)
			perror("tcsetattr ~ICANON");
	
		return buf;
	#endif
}
