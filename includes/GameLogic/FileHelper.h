#pragma once
#include "GameUtils.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <json_fwd.hpp>

struct GameplayStep
{
	int boardLength;
	std::vector<int> view;
	SnakeMove move;
	int snakeHeadPos;
	int snakeNeckPos;

	GameplayStep(int boardLength, std::vector<int> view, SnakeMove move, int snakeHeadPos, int snakeNeckPos)
	:boardLength(boardLength), view(view), move(move), snakeHeadPos(snakeHeadPos), snakeNeckPos(snakeNeckPos){}
	GameplayStep() {}
};

void from_json(const nlohmann::json& j, GameplayStep& gmStep);
void to_json(nlohmann::json& j, const GameplayStep& gmStep);

class FileHelper 
{
public: 
	FileHelper(const std::string& filename);
	~FileHelper();

	void WriteToFile(const GameplayStep& gameplayStep);
private:
	std::fstream m_file;
	std::string m_fileName;
};
