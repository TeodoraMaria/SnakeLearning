#include "AI/GeneticAlgorithm/GeneticTrainerMultiSnake.h"

#include "GameLogic/CellInterpreter/Basic3CellInterpreter.hpp"
#include "GameLogic/CellInterpreter/WallFoodBody.hpp"
#include "GameLogic/CellInterpreter/WallFoodEnemy.hpp"

#include "GymEnv/StateObserver/GridObserver.hpp"
#include "GymEnv/StateObserver/ThreeDirectionalObserver.hpp"
#include "GymEnv/StateObserver/FoodDirectionDecorator.hpp"
#include "Utils/MathUtils.h"

#include <memory>
#include <thread>
#include <qcoreapplication.h>
#include <QtConcurrent/qtconcurrentrun.h>
#include <qfuturesynchronizer.h>


using namespace AI::GeneticAlgorithm;
using namespace GameLogic::CellInterpreter;
using namespace GymEnv::StateObserver;


GeneticTrainerMultiSnake::GeneticTrainerMultiSnake()
{
}

GeneticTrainerMultiSnake::~GeneticTrainerMultiSnake()
{
    int x = 0;
}

void GeneticTrainerMultiSnake::displayBestNetwork()
{
    const GeneticNetwork* bestNetwork;
    double maxFitness = 0.0;
    for (const auto& network : m_networks) {
        if (maxFitness < network.getFitness()) {
            maxFitness = network.getFitness();
            bestNetwork = &network;
        }
    }
    auto player = std::make_shared<GeneticBot>(*bestNetwork, std::shared_ptr<GymEnv::StateObserver::IStateObserver>(m_observer));
}

IPlayerPtr GeneticTrainerMultiSnake::Train(TrainCallbacks callbacks)
{
    setup();

    for (size_t i = 0; i < callbacks.numEpisodes; i++) {
        runEpisode(i);

        if(callbacks.emitDisplayGame)
        {
            const GeneticNetwork* bestNetwork;
            double maxFitness = 0.0;
            for (const auto& network : m_networks) {
                if (maxFitness < network.getFitness()) {
                    maxFitness = network.getFitness();
                    bestNetwork = &network;
                }
            }
            auto player = std::make_shared<GeneticBot>(*bestNetwork, std::shared_ptr<GymEnv::StateObserver::IStateObserver>(m_observer));

            callbacks.emitDisplayGame(player, m_options.maxNumSteps);
        }

        if (callbacks.emitGraphValues)
        {
            double maxFitness = 0;
            double totalFitness = 0;
            for (const auto& network : m_networks) {

                double networkFitness = std::cbrt(network.getFitness());

                if (maxFitness < networkFitness) {
                    maxFitness = networkFitness;
                }
                totalFitness += networkFitness;
            }
        
            std::vector<double> values = {maxFitness,totalFitness / m_networks.size()};

            callbacks.emitGraphValues(values);
        }
        selectNewNetworks();
        crossover();
        mutate();

        if (i < callbacks.numEpisodes - 1) {
            resetFitness();       
        }

        if (callbacks.emitStepEpisode) {
            callbacks.emitStepEpisode(i+1);
        }
    }

    const GeneticNetwork* bestNetwork;
    double maxFitness = 0.0;
    for (const auto& network : m_networks) {
        if (maxFitness < network.getFitness()) {
            maxFitness = network.getFitness();
            bestNetwork = &network;
        }
    }

    return IPlayerPtr(new GeneticBot(*bestNetwork, std::shared_ptr<GymEnv::StateObserver::IStateObserver>(m_observer)));
}

void GeneticTrainerMultiSnake::setEpisodes(size_t episodes)
{
    m_options.numEpisodes = episodes;

}


void GeneticTrainerMultiSnake::setup()
{
    auto cellInterpreter = std::make_shared<Basic3CellInterpreter>();
    m_observer = std::make_shared<GridObserver>(cellInterpreter, 7, 7);
    //m_observer = std::make_shared<ThreeDirectionalObserver>(cellInterpreter);

    m_options.crossoverProb = 0.5;
    m_options.maxNumSteps = 100;
    m_options.mutationProb = 0.1;
    //m_options.numEpisodes = 1000;
    m_options.numOfNetworks = 50;

    m_networkSettings.m_inputs = m_observer.get()->NbOfObservations() + 1;
    m_networkSettings.m_hiddenLayersSizes = {50,10,3};

    m_networks.resize(m_options.numOfNetworks);

    for (auto& network : m_networks) {
        network = GeneticNetwork(m_networkSettings);
        network.initWeights();
    }
}

void GeneticTrainerMultiSnake::runEpisode(size_t episode)
{
    static GameOptions options;
    options.boardLength = 25;
    options.boardWidth = 25;
    options.numFoods = 10;
    

    QFutureSynchronizer<void> synchronizer;

    auto networkGame = [&](GeneticNetwork& network) {

        auto geneticBot = GeneticBot(network, m_observer);
        std::vector<IPlayerPtr> players;
        players.push_back(std::make_shared<GeneticBot>(geneticBot));
        Game* game = new Game(options, players);
        //  m_game= new Game(options, players);
        game->InitGame();

        for (size_t i = 0; i < m_options.maxNumSteps; i++) {
            auto currentScore = game->GetAllSnakes().at(0).GetScore();
            game->RunRound();

            if (game->GetAllSnakes().at(0).GetScore() != currentScore) {
                i = 0;
            }
            if (game->EveryoneIsDead()) {
                break;
            }
        }
        network.updateFitness(game->GetAllSnakes().at(0).GetScore());
        delete game;
    };

    for (GeneticNetwork& network : m_networks) {
        QFuture<void> future = QtConcurrent::run(networkGame,std::ref(network));
        synchronizer.addFuture(future);
    }
    synchronizer.waitForFinished();
    synchronizer.clearFutures();
}

void GeneticTrainerMultiSnake::crossover()
{
    //check for 2 networks at a time
    double crossoverValue;
    for (size_t i = 0; i < m_options.numOfNetworks; i += 2) {

        crossoverValue = Utils::Math::randomDouble(0.00000001, 1.0);
        if (crossoverValue < m_options.crossoverProb) {
            m_networks[i].crossover(m_networks[i + 1]);
        }
    }
}

void GeneticTrainerMultiSnake::selectNewNetworks()
{
    //get the total fitness
    double fitnessSum = 0.0;
    for (const auto& network : m_networks) {
        fitnessSum += network.getFitness();
    }

    //compute the probabilitly for each network to be selected
    double selectionProb = m_networks[0].getFitness() / fitnessSum;
    m_networks[0].setSelectionProb(selectionProb);

    double cumulativeProb = 0.0;
    for (size_t i = 1; i < m_options.numOfNetworks; i++) {
        selectionProb = m_networks[i].getFitness() / fitnessSum;

        //Pci = Pci-1 + Psi;
        cumulativeProb = m_networks[i - 1].getSelectionProb() + selectionProb;
        m_networks[i].setSelectionProb(cumulativeProb);
    }
    std::vector<GeneticNetwork> newNetworks;

    newNetworks.resize(m_networks.size());
    //generate numbers between (0,1]
    double selectionValue = 0.0;
    for (size_t i = 0; i < m_options.numOfNetworks; i++) {
        selectionValue = Utils::Math::randomDouble(0.00000001, 1.0);

        GeneticNetwork selectedNetwork;

        for (size_t j = 0; j < m_options.numOfNetworks; j++) {
            if (selectionValue <= m_networks[j].getSelectionProb()) {
                selectedNetwork = m_networks[j];
                break;
            }
        }
        newNetworks[i] = selectedNetwork;
    }
    m_networks = newNetworks;
}

void GeneticTrainerMultiSnake::mutate()
{
    for (auto& network : m_networks) {
        network.mutateWeights(m_options.mutationProb);
    }
}

void GeneticTrainerMultiSnake::printFitnessInfo(size_t episode)
{  

}

void GeneticTrainerMultiSnake::resetFitness()
{
    for (auto& network : m_networks) {
        network.setFitness(0);
    }
}



