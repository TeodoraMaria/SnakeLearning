#include "Getch.h"
#include <iostream>

#ifdef _WIN32
	#include <windows.h>
	#include <conio.h>
#else
	#include <unistd.h>
	#include <termios.h>
#endif

#ifdef _WIN32
	int WindowsGetch()
	{
		const auto result = ::getch();
		::getch();
		return result;
	}
#else
	int UnixGetch()
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

int MultiplatformUtils::Getch()
{
	#ifdef _WIN32
		return WindowsGetch();
	#else
		return UnixGetch();
	#endif
}

