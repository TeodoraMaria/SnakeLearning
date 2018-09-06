#pragma once
#include "GameUtils.h"
#include <vector>
#include <fstream>
#include <iostream>

class FileHelper 
{
public: 
	FileHelper(const std::string& filename);
	~FileHelper();

	void WriteToFile(const std::vector<int> view, SnakeMove move, const int snakeNumber);
private:
	std::ofstream m_file;
};