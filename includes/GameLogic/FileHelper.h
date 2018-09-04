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

	void WriteToFile(const std::vector<std::vector<int>> view, SnakeMove move);
private:
	std::ofstream m_file;
};