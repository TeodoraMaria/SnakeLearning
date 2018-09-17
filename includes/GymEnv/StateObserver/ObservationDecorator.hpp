#pragma once

#include "IStateObserver.hpp"

namespace GymEnv { namespace StateObserver
{
	class ObservationDecorator : public IStateObserver
	{
	public:
		ObservationDecorator(
			GameLogic::CellInterpreter::ICellInterpreterPtr cellInterpreter,
			std::shared_ptr<IStateObserver> wrapee);
	
		virtual void Observe(
			std::vector<double>& observationContainer,
			const GameState& gmState,
			int snakeId) const override;
	
	protected:
		std::shared_ptr<IStateObserver> m_wrapee;
	};
}}


