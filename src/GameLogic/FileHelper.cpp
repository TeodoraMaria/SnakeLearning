#include "FileHelper.h"

FileHelper::FileHelper(const std::string& filename)
{
	m_file.open(filename, std::fstream::out);
}

FileHelper::~FileHelper()
{
	m_file.close();
}

void FileHelper::WriteToFile(const std::vector<int> view, SnakeMove move, const int snakeNumber)
{
	for(const auto& elem: view)
	{
		int val = 0;
		if (elem == 0)
		{
			val = 0;
		}
		else if (elem == 1)
		{
			val = -2;
		}
		else if (elem == snakeNumber)
		{
			val = 1;
		}
		else
		{
			val = -1;
		}
		m_file << val <<" ";
	}
	m_file << "," << static_cast<int>(move)<<std::endl;
}
