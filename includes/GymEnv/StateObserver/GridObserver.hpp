#pragma once

#include "IStateObserver.hpp"
#include "GameLogic/CellInterpreter/ICellInterpreter.hpp"

namespace GymEnv { namespace StateObserver
{
	class GridObserver : public IStateObserver
	{
	public:
		GridObserver(
			GameLogic::CellInterpreter::ICellInterpreterPtr cellInterpreter,
			size_t width,
			size_t height,
			const Coordinate& deltaPos = Coordinate(0, 0));
	
		size_t GetWidth() const;
		size_t GetHeight() const;
	
		size_t NbOfObservations() const override;
		void Observe(
			std::vector<double>& observationContainer,
			const GameState& gmState,
			int snakeId) const override;
		
		IStateObserver* Clone() const override;
		
	private:
		GameLogic::CellInterpreter::ICellInterpreterPtr m_cellInterpreter;
		const size_t m_width;
		const size_t m_height;
		const Coordinate m_deltaPos;
	};
	
	typedef std::unique_ptr<GridObserver> GridObserverUPtr;
}}
