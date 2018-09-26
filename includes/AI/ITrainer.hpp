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
            // For loading bar.
            std::function<void(size_t episode)> emitStepEpisode;
            std::function<void(std::vector<double>)> emitGraphValues;
            std::function<void(IPlayerPtr, size_t)> emitDisplayGame;

            size_t numEpisodes;
			size_t fieldX;
			size_t fieldY;
        };

		virtual IPlayerPtr Train(TrainCallbacks callbacks) = 0;
	};
}
