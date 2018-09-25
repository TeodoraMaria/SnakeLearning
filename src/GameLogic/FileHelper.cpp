#include "FileHelper.h"
#include <json.hpp>
#include <iomanip>

FileHelper::FileHelper(const std::string& filename):m_fileName(filename)
{
	/*m_file.open(filename, std::fstream::out);
	m_file << "[]";
	m_file.close();*/
	m_file.open(filename, std::fstream::in);
}

FileHelper::~FileHelper()
{
	m_file.close();
}

void from_json(const nlohmann::json& j, GameplayStep& gmStep)
{
	gmStep.boardLength = j.at("boardLength").get<int>();
	gmStep.move = j.at("move").get<SnakeMove>();
	gmStep.snakeHeadPos = j.at("snakeHeadPos").get<int>();
	gmStep.snakeNeckPos = j.at("snakeNeckPos").get<int>();
	gmStep.view = j.at("view").get<std::vector<int>>();
}

void to_json(nlohmann::json& j, const GameplayStep& gmStep)
{
	j = nlohmann::json{
		{ "boardLength", gmStep.boardLength },
		{ "view", gmStep.view },
		{ "snakeHeadPos", gmStep.snakeHeadPos },
		{ "snakeNeckPos", gmStep.snakeNeckPos },
		{ "move", static_cast<int>(gmStep.move) }
	};
}

void FileHelper::WriteToFile(const GameplayStep& gameplayStep)
{
	nlohmann::json j;

	m_file >> j;
	
	std::vector<GameplayStep> gmSteps = j.get< std::vector<GameplayStep>>();
	gmSteps.push_back(gameplayStep);
	m_file.close();
	m_file.open(m_fileName, std::fstream::out);

	m_file << nlohmann::json(gmSteps);

	/*m_file << boardLength << ",";
	for(const auto& elem: view)
	{
		m_file << elem <<" ";
	}
	m_file<<","<< snakeHeadPos<<" "<<snakeNeckPos<< "," << static_cast<int>(move)<<std::endl;*/
}
