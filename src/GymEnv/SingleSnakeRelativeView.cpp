#include "SingleSnakeRelativeView.hpp"
#include <cmath>

using namespace GymEnv;

const std::array<int, 3> SingleSnakeRelativeView::actions = {{-1, 0, 1}};

/*
** 2^3 * 4
*/

size_t SingleSnakeRelativeView::GetNumbOfObservations() const
{
	static const size_t viewGridSize = 3;
	
	// 0 - Nothing, 1 - Wall, 2 - Food.
	static const size_t cellStatesCount = 3;
	
	return std::pow(viewGridSize, cellStatesCount);
}

SnakeState SingleSnakeRelativeView::GetState() const
{
	int state = 0;
	
	/*
		for cell, cell_index in viewGrid:
			if cell != 0:
				state += cell * 3 ^ cell_index
	 
		Where cell is { 0, WALL = 1, FOOD = 2 }
	*/
	assert(state < GetNumbOfObservations());
 	return state;
}
