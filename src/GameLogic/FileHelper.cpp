#include "FileHelper.h"

FileHelper::FileHelper(const std::string& filename)
{
	m_file.open(filename, std::fstream::out);
}

FileHelper::~FileHelper()
{
	m_file.close();
}

void FileHelper::WriteToFile(const std::vector<std::vector<int>> view, SnakeMove move)
{
	for(const auto& line: view)
		for (const auto& elem : line)
		{
			m_file << elem << " ";
		}
	m_file << std::endl << static_cast<int>(move)<<std::endl;
}
