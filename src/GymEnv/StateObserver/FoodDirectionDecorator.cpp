#include "StateObserver/FoodDirectionDecorator.hpp"
#include <iostream>
#include <assert.h>

using namespace GymEnv::StateObserver;

FoodDirectionDecorator::FoodDirectionDecorator(
	GameLogic::CellInterpreter::ICellInterpreterPtr cellInterpreter,
	std::shared_ptr<IStateObserver> wrapee
) :
	ObservationDecorator(cellInterpreter, wrapee)
{
}

size_t FoodDirectionDecorator::NbOfObservations() const
{
	return m_wrapee->NbOfObservations() + 2;
}

/*
** Not sure about this implementation.
*/

void FoodDirectionDecorator::Observe(
	std::vector<double>& obserContainer,
	const GameState& gmState,
	const int snakeId) const
{
	assert(obserContainer.size() >= NbOfObservations());
	ObservationDecorator::Observe(obserContainer, gmState, snakeId);
	
	const auto snake = gmState.GetSnake(snakeId);
	const auto snakeHead = snake.GetSnakeHead();
	const auto closestFood = gmState.GetGameBoard().FindClosestFood(snakeHead);
	
	const auto directionToFood = closestFood - snakeHead;
	const auto magnitude = directionToFood.Magnitude();
	
	double normX;
	double normY;
	
	if (directionToFood.GetX() == 0 && directionToFood.GetY() == 0)
	{
		normX = 0;
		normY = 0;
	}
	else
	{
		normX = directionToFood.GetX() / magnitude;
		normY = directionToFood.GetY() / magnitude;
	}
//	std::sqr
//	const auto snakeOrientation = snake.GetOrientation();
//
//	const auto dirX = (double)normX - (double)snakeOrientation.GetX();
//	const auto dirY = (double)normY - (double)snakeOrientation.GetY();
//	obserContainer[m_wrapee->NbOfObservations() + 0] = dirX;
//	obserContainer[m_wrapee->NbOfObservations() + 1] = dirY;

	obserContainer[m_wrapee->NbOfObservations() + 0] = normX;
	obserContainer[m_wrapee->NbOfObservations() + 1] = normY;
	
//	std::cout << dirX << " : " << dirY << std::endl;
//	std::cout << normX << " : " << normY << std::endl;
}
