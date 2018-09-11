#include "StateObserver/GridObserver.hpp"
#include <assert.h>

using namespace GymEnv::StateObserver;

GridObserver::GridObserver(
	GameLogic::CellInterpreter::ICellInterpreterPtr cellInterpreter,
	size_t width,
	size_t height,
	const Coordinate& deltaPos
) :
	m_cellInterpreter(cellInterpreter),
	m_width(width),
	m_height(height),
	m_deltaPos(deltaPos)
{
}

size_t GridObserver::GetWidth() const { return m_width; }
size_t GridObserver::GetHeight() const { return m_height; }

size_t GridObserver::NbOfObservations() const
{
	return
		(m_width * m_height) *
		(m_cellInterpreter->NbOfInterpretableParts() - 1);
}

void GridObserver::Observe(
	std::vector<double>& observationTab,
	const GameState& gmState,
	const int snakeId) const
{
	assert(observationTab.size() >= NbOfObservations());
	
	const auto snake = gmState.GetSnake(snakeId);
	assert(snake.IsAlive());
	
	const auto gmBoard = gmState.GetGameBoard();
	
	const auto forward = snake.GetOrientation();
	const auto left = forward.Rotate90Left();
	const auto right = forward.Rotate90Right();
	
	const auto origin =
		snake.GetSnakeHead() + forward * m_deltaPos.GetX() + left * m_deltaPos.GetY()  + left * (m_height / 2);
	
	std::fill(observationTab.begin(), observationTab.end(), 0);
	
	auto startOfIndex = 0;
	for (auto x = 0; x < m_height; x++)
		for (auto y = 0; y < m_width; y++)
		{
			const auto pos = origin + right * x + forward * y;
			const auto cellVal = gmBoard.CoordIsBounded(pos) ?
				static_cast<BoardPart>(gmBoard[pos]) : BoardPart::WALL;
			
			const auto indx =
				startOfIndex *
				(m_cellInterpreter->NbOfInterpretableParts() - 1);

			const auto interpretVal = m_cellInterpreter->InterpretCell(snakeId, cellVal);
			
			assert(indx <= observationTab.size());
			if (interpretVal != 0)
				observationTab[indx + interpretVal - 1] = 1;
			
			startOfIndex++;
		}
}

IStateObserver* GridObserver::Clone() const
{
	return new GridObserver(
		m_cellInterpreter,
		m_width,
		m_height,
		m_deltaPos);
}
