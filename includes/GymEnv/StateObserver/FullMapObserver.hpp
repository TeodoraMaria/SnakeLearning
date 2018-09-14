#pragma once

#include "IStateObserver.hpp"

namespace GymEnv { namespace StateObserver
{
	class FullMapObserver : public IStateObserver
	{
	public:
		FullMapObserver(
			GameLogic::CellInterpreter::ICellInterpreterPtr cellInterpreter,
			size_t maxMapCells);

		size_t NbOfObservations() const override;
		void Observe(
			std::vector<double>& observationContainer,
			const GameState& gmState,
			int snakeId) const override;

	private:
		const size_t m_maxMapCells;
	};
}}

