#include "StateObserver/ThreeDirectionalObserver.hpp"
#include "Utils/PrintUtils.h"
#include <assert.h>

using namespace GymEnv::StateObserver;

ThreeDirectionalObserver::ThreeDirectionalObserver(
	GameLogic::CellInterpreter::ICellInterpreterPtr cellInterpreter,
	size_t leftFieldSize,
	size_t forwardFieldSize,
	size_t rightFieldSize
) :
	m_cellInterpreter(cellInterpreter),
	m_leftFieldSize(leftFieldSize),
	m_forwardFieldSize(forwardFieldSize),
	m_rightFieldSize(rightFieldSize)
{
}

size_t ThreeDirectionalObserver::NbOfObservations() const
{
	return 3 * (m_cellInterpreter->NbOfInterpretableParts() - 1);
}

/*
**	For i : range(3):
**		detectedObjIndx = raycast(directions[i])
**
** 		if detected anything:
**			result[i * totalNbOfInterpretables + detectedObjIndx] = dist to obj
** Where result is the given container.
** The distance is reversed: 1.0 for being very close.
*/

void ThreeDirectionalObserver::Observe(
	std::vector<double>& observationContainer,
	const GameState& gmState,
	const int snakeId) const
{
	assert(observationContainer.size() >= NbOfObservations());
	
	const auto snake = gmState.GetSnake(snakeId);
	
	const auto forward = snake.GetOrientation();
	const auto left = forward.Rotate90Left();
	const auto right = forward.Rotate90Right();
	
	const auto nbOfInterpretables =
		m_cellInterpreter->NbOfInterpretableParts() - 1;
	
	// Put the 'distance' in the detected part index, if any detected.
	const auto putDist = [&](
		const Coordinate& direction,
		const size_t dist,
		const size_t i)
	{
		if (dist == 0)
			return;

		const auto rayCastRs = InterpretRayCast(
			gmState.GetGameBoard(),
			snake.GetSnakeHead(),
			direction,
			dist,
			snakeId);
		
		if (rayCastRs.detectedPart == 0)
			return;
		
		// 1.0 for being very close.
		const auto normalizedDist = 1.0 - (double)rayCastRs.distance / dist;
		
		const auto startOfInputIndx = i * nbOfInterpretables;
		const auto detectedPartIndx =
			startOfInputIndx + rayCastRs.detectedPart - 1;
		
		assert(detectedPartIndx < observationContainer.size());
		observationContainer[detectedPartIndx] = normalizedDist;
	};
	
	std::fill(observationContainer.begin(), observationContainer.end(), 0);
	putDist(left, m_leftFieldSize, 0);
	putDist(forward, m_forwardFieldSize, 1);
	putDist(right, m_rightFieldSize, 2);
}

IStateObserver* ThreeDirectionalObserver::Clone() const
{
	return new ThreeDirectionalObserver(
		m_cellInterpreter,
		m_leftFieldSize,
		m_forwardFieldSize,
		m_rightFieldSize);
}

/*
** Private methods.
*/

ThreeDirectionalObserver::InterpretRayCastResult
ThreeDirectionalObserver::InterpretRayCast(
	const GameBoard& gmBoard,
	const Coordinate& origin,
	const Coordinate& direction,
	const size_t distance,
	const int playerId) const
{
	auto result = InterpretRayCastResult();
	
	for (auto i = 0u; i < distance; i++)
	{
		const auto pos = origin + direction * (i + 1);
		const auto boardPart = static_cast<BoardPart>(gmBoard[pos]);
		const auto interpretationId =
			m_cellInterpreter->InterpretCell(playerId, boardPart);
		
		if (interpretationId != 0)
		{
			result.detectedPart = interpretationId;
			result.distance = i;
			return result;
		}
	}
	
	result.detectedPart = 0;
	result.distance = distance;
	return result;
}
