#pragma once
#include "GameLogic/IPlayer.h"
#include "AI/Supervised/SupervisedBot.h"
#include "AI/Supervised/SupervisedTrainer.h"

namespace AI {
	namespace Supervised {
		enum TrainingWay {BASIC, ENEMY, FULL};
		class SupervisedManager {
		public:
			SupervisedManager();
			~SupervisedManager();
			IPlayerPtr GetSupervisedBot(const int fieldX, const int fieldY, const TrainingWay trainingWay);
			void TrainSupervisedBot(const std::string& inputFilePath, const int fieldX, const int fieldY, const TrainingWay trainingWay);
		private:
			SupervisedBot* m_bot;

			void LoadSupervisedBot(std::string fileName);
			void SaveSupervisedBot() const;
			TrainingData GetTrainingData(const int fieldX, const int fieldY, const std::string & inputFilePath = "D:\\SnakeData.txt", const TrainingWay trainingWay = TrainingWay::BASIC) const;
			std::vector<int> GetFieldOfView(const std::vector<int> map, const int snakeHead, const int fieldX, const int fieldY, const int cols) const;

			int BasicTranslate(int x) const;
			int EnemyTranslate(int x) const;
			int FullTranslate(int x, int snakeNumber) const;
			void Normalize(std::vector<double>& inputs) const;
			std::vector<int> OneHotEncoder(int x) const;

			void InitializeBot(const int fieldX, const int fieldY);
		};
	}
}