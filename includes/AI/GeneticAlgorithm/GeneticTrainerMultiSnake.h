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
#include <vector>
#include <memory>


namespace AI
{
    namespace GeneticAlgorithm{
    
        class GeneticTrainerMultiSnake:public QObject,public ITrainer
        {
            Q_OBJECT
        public:
            GeneticTrainerMultiSnake();
            ~GeneticTrainerMultiSnake();

            virtual IPlayer * Train() override;

            void setup();
            void runEpisode(size_t episode);
            double runStep(const std::vector<double>& state, const GeneticNetwork& network);
            void crossover();
            void selectNewNetworks();
            void mutate();
            void printFitnessInfo(size_t episode);
            void resetFitness();

        signals:
            void graphValues(const std::vector<double>& values);
        private:

            void emitGraphValues(const std::vector<double>& values);
            Game* m_game;
            Utils::NetworkSettings m_networkSettings;
            GeneticOptions m_options;
            std::vector<GeneticNetwork> m_networks;

            std::vector<IPlayerPtr> m_players;

           // std::shared_ptr<GymEnv::StateObserver::GridObserver> m_observer=nullptr;

            std::shared_ptr<GymEnv::StateObserver::ThreeDirectionalObserver> m_observer = nullptr;

            QtCharts::QLineSeries* m_maxFitness;
            QtCharts::QLineSeries* m_avgFitenss;


        };
    }
}