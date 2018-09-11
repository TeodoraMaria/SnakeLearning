#pragma once

#include "IStateObserver.hpp"
#include "GameLogic/CellInterpreter/ICellInterpreter.hpp"

namespace GymEnv { namespace StateObserver
{
	class ThreeDirectionalObserver : public IStateObserver
	{
	public:
		ThreeDirectionalObserver(
			GameLogic::CellInterpreter::ICellInterpreterPtr cellInterpreter,
			size_t leftFieldSize = 1,
			size_t forwardFieldSize = 1,
			size_t rightFieldSize = 1
		);
	
		size_t NbOfObservations() const override;
		void Observe(
			std::vector<double>& observationContainer,
			const GameState& gmState,
			int snakeId) const override;
		
		IStateObserver* Clone() const override;
		
	private:
		struct InterpretRayCastResult;
	
		InterpretRayCastResult InterpretRayCast(
			const GameBoard& gmBoard,
			const Coordinate& origin,
			const Coordinate& direction,
			size_t distance,
			int playerId) const;
	
		GameLogic::CellInterpreter::ICellInterpreterPtr m_cellInterpreter;
		const size_t m_leftFieldSize;
		const size_t m_forwardFieldSize;
		const size_t m_rightFieldSize;
	};
	
	typedef std::unique_ptr<ThreeDirectionalObserver> ThreeDirectionalObserverUPtr;
	
	struct ThreeDirectionalObserver::InterpretRayCastResult
	{
		size_t detectedPart;
		size_t distance;
	};
}}
