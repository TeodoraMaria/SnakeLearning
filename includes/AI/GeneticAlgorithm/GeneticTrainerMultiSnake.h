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
    
        class GeneticTrainerMultiSnake:public QObject,public ITrainer
        {
            Q_OBJECT
        public:
            GeneticTrainerMultiSnake();
            ~GeneticTrainerMultiSnake();

            virtual IPlayer * Train() override;

            void setEpisodes(size_t episodes);

        protected slots:
            void switchDisplayEnabled();
            void endGame();
            

        signals:
            void loadingBar(double value);
            void graphValues(const std::vector<double>& values);
            void gameState(GameState gamestate);

        private:
            void setup();
            void runEpisode(size_t episode);
            void crossover();
            void selectNewNetworks();
            void mutate();
            void printFitnessInfo(size_t episode);
            void resetFitness();

            void displayBestNetwork();
            void emitGraphValues(const std::vector<double>& values);
            void emitLoadingBar(size_t);

            bool m_displayEnabled=false;

            Game* m_game;
            Utils::NetworkSettings m_networkSettings;
            GeneticOptions m_options;
            std::vector<GeneticNetwork> m_networks;

            std::vector<IPlayerPtr> m_players;

            std::shared_ptr<GymEnv::StateObserver::GridObserver> m_observer=nullptr;

            //std::shared_ptr<GymEnv::StateObserver::ThreeDirectionalObserver> m_observer = nullptr;

            QtCharts::QLineSeries* m_maxFitness;
            QtCharts::QLineSeries* m_avgFitenss;

            QTimer m_timer;
        };
    }
}