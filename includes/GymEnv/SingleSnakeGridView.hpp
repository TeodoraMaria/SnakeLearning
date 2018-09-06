#pragma once

#include "SingleSnakeEnvBase.hpp"
#include "StepResult.h"
#include "SnakeStudent.hpp"
#include "StateExtractor.hpp"
#include "GameView/IGameRenderer.hpp"
#include "GymEnv/StateObserver/GridObserver.hpp"
#include <cstddef>
#include <vector>

/*
** An environment where the snake sees one position ahead.
*/

namespace GymEnv
{
	struct SingleSnakeGridViewModel
	{
		SingleSnakeEnvBaseModel baseModel;
		size_t gridWidth = 3;
		size_t gridHeight = 3;
		Coordinate deltaCoord = Coordinate(0, 0);
	};
	
	class SingleSnakeGridView : public SingleSnakeEnvBase
	{
	public:
		SingleSnakeGridView(const SingleSnakeGridViewModel& model);
		
		const StateObserver::IStateObserver* GetObserver() const override;
		size_t GetNumbOfObservations() const override;
		std::vector<double> GetState() const override;
	
	private:
		StateObserver::GridObserverUPtr m_gridObserver;
	};
}
