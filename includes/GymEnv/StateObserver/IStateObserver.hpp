#pragma once

#include "GameLogic/GameState.h"
#include <cstddef>
#include <vector>

namespace GymEnv { namespace StateObserver
{
	class IStateObserver
	{
	public:
		virtual size_t NbOfObservations() const = 0;
		virtual void Observe(
			std::vector<double>& observationContainer,
			const GameState& gmState,
			int snakeId) const = 0;
	};
}}
