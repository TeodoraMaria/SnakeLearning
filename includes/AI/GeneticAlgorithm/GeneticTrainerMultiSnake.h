#pragma once

#include "AI/ITrainer.hpp"
#include "GymEnv/SingleSnakeEnvBase.hpp"
#include "GeneticOptions.h"
#include "GeneticNetwork.h"
#include "GeneticBot.h"
#include "GymEnv/StateObserver/GridObserver.hpp"
#include "GymEnv/StateObserver/ThreeDirectionalObserver.hpp"

#include <QtCharts/qlineseries.h>
#include <qobject.h>
#include <qtimer.h>
#include <vector>
#include <memory>


namespace AI
{
    namespace GeneticAlgorithm{
    
        class GeneticTrainerMultiSnake : public ITrainer
        {
        public:
            GeneticTrainerMultiSnake();
            ~GeneticTrainerMultiSnake();

            virtual IPlayerPtr Train(TrainCallbacks callbacks) override;

            void setEpisodes(size_t episodes);
        private:
            void setup();
            void runEpisode(size_t episode);
            void crossover();
            void selectNewNetworks();
            void mutate();
            void printFitnessInfo(size_t episode);
            void resetFitness();

            void displayBestNetwork();
            
            Utils::NetworkSettings m_networkSettings;
            GeneticOptions m_options;
            std::vector<GeneticNetwork> m_networks;

            std::vector<IPlayerPtr> m_players;

            std::shared_ptr<GymEnv::StateObserver::GridObserver> m_observer;
        };
    }
}
