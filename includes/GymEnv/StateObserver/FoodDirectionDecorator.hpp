#pragma once

#include "ObservationDecorator.hpp"

namespace GymEnv { namespace StateObserver
{
	class FoodDirectionDecorator : public ObservationDecorator
	{
	public:
		FoodDirectionDecorator(
			GameLogic::CellInterpreter::ICellInterpreterPtr cellInterpreter,
			std::shared_ptr<IStateObserver> wrapee);
	
		size_t NbOfObservations() const override;
		void Observe(
			std::vector<double>& observationContainer,
			const GameState& gmState,
			int snakeId) const override;
	};
}}
