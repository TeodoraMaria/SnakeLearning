#pragma once

#include "GameLogic/IPlayer.h"
#include <functional>

namespace AI
{
	class ITrainer
	{
	public:
        struct TrainCallbacks
        {
            std::function<void(size_t episode)> emitStepEpisode;
            std::function<void(std::vector<double>)> emitGraphValues;
            std::function<void(IPlayerPtr, size_t)> emitDisplayGame;

            size_t numEpisodes;
        };

		virtual IPlayer* Train(TrainCallbacks callbacks) = 0;
	};
}
