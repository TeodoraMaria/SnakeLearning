#include "AI/Supervised/SupervisedManager.h"
#include "GameLogic/Game.h"

int main(int argc, char** argv)
{
	SupervisedManager sm;
	sm.TrainSupervisedBot("D:\SnakeData.txt", 3, 3, AI::Supervised::TrainingWay::BASIC);

	GameOptions options;
	options.boardLength = 25;
	options.boardWidth =25;
	options.numFoods = 5;

	//std::vector<IPlayerPtr> players = { sm.GetSupervisedBot(3, 3, AI::Supervised::TrainingWay::BASIC) };

	//Game game(options, players);

	//game.Play();
	
	return 0;
}
