#include "FileHelper.h"

FileHelper::FileHelper(const std::string& filename)
{
	m_file.open(filename, std::ios::app);
}

FileHelper::~FileHelper()
{
	m_file.close();
}

void FileHelper::WriteToFile(const int boardLength, const std::vector<int> view, SnakeMove move, const int snakeHeadPos, const int snakeNeckPos)
{
	m_file << boardLength << ",";
	for(const auto& elem: view)
	{
		m_file << elem <<" ";
	}
	m_file<<","<< snakeHeadPos<<" "<<snakeNeckPos<< "," << static_cast<int>(move)<<std::endl;
}
