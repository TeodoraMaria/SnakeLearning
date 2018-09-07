#include "AI/GeneticAlgorithm/GeneticTrainer.h"
#include "Utils/MathUtils.h"
#include <iostream>


using namespace AI::GeneticAlgorithm;

GeneticTrainer::GeneticTrainer(GeneticOptions options, GymEnv::SingleSnakeEnvBase* env):
    m_options(options),m_env(env)
{
    m_networks.resize(m_options.numOfNetworks);
    
}

IPlayer *GeneticTrainer::Train()
{
    //on each train reset the networks;
    for (auto& network : m_networks) {
        network.initWithRandomValues();
    }

    for (size_t i = 0; i < m_options.numEpisodes; i++) {
        runEpisode();

        selectNewNetworks();
        crossover();
        mutate();


        std::cout << i << std::endl;
    }
    return nullptr;
}

void GeneticTrainer::runEpisode()
{
    for (auto& network : m_networks) {
        
        runBot(network);
        //run game for each bot;
    }

}

double GeneticTrainer::runStep(const std::vector<double>& state, const GeneticNetwork& network)
{
    SnakeMove move=network.feedForward(state);
    GymEnv::StepResult result=m_env->Step(move);

    double reward = 0.0;
    // change to enum
    switch (result.reward) {
        case -1: {
            reward = -1000.0;
            break;
        }
        case 0: {
           // reward = 0.0;
            break;
        }
        case 1: {
            reward = 1.0;
            break;
        }
        default: {
            break;
        }
    }
    return reward;
}

void GeneticTrainer::runBot(GeneticNetwork& network)
{
    m_env->Reset();
    std::vector<double> state=m_env->GetState();

    for (size_t i = 0; i < m_options.maxNumSteps; i++) {

       double reward=runStep(state,network);
       if (reward == -1000.0) {
           break;
       }
       
       //dead change to snake lenght
       network.updateFitness(reward);
       state= m_env->GetState();
    }
}

void GeneticTrainer::crossover()
{
    //check for 2 networks at a time
    double crossoverValue;
    for (size_t i = 0; i < m_options.numOfNetworks; i += 2) {

        crossoverValue = Utils::Math::randomDouble(0.00000001, 1.0);
        if (crossoverValue < m_options.crossoverProb) {
            //TODO:crossover
            //something.crossover(m_networks[i],m_networks[i+1]);
        }
    }
}
void GeneticTrainer::selectNewNetworks()
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

    //generate numbers between (0,1]
    double selectionValue = 0.0;
    for (size_t i = 0; i < m_options.numOfNetworks; i++) {
        selectionValue = Utils::Math::randomDouble(0.00000001, 1.0);

        for (size_t j = 0; j < m_options.numOfNetworks-1; j++) {
            if (selectionValue > m_networks[j+1].getSelectionProb()) {
                m_networks[j].selectForCrossOver(true);
                break;
            }
        }
    }
}


void GeneticTrainer::mutate()
{
    double mutationValue;
    for (auto& network : m_networks) {
            //TODO:mutation;
            //network.mutate?

        mutationValue = Utils::Math::randomDouble(0.00000001, 1.0);
        if (mutationValue < m_options.mutationProb) {
        }
    }

}


