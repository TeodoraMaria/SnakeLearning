#include "AI/QLearning/TabularTrainer.hpp"
#include <stdlib.h>
#include <time.h>

int main()
{
	srand(time(nullptr));
	
	auto trainer = AI::QLearning::TabularTrainer();
	trainer.Train();
	return 0;
}
