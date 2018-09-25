#include "AI/Supervised/SupervisedManager.h"

int main(int argc, char** argv)
{
	SupervisedManager sm;
	sm.TrainSupervisedBot("D:\SnakeData.txt", 3, 3, AI::Supervised::TrainingWay::BASIC);
	return 0;
}
