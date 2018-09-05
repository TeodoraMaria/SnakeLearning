#include "AI/GeneticAlgorithm/GeneticTrainer.h"
#include "Utils/MathUtils.h"


using namespace AI::GeneticAlgorithm;

GeneticTrainer::GeneticTrainer(GeneticOptions options, GymEnv::SingleSnakeEnvBase* env):
    m_options(options),m_env(env)
{
  
    m_networks.resize(m_options.numOfNetworks);
    m_networkProb.resize(m_options.numOfNetworks);

}

IPlayer *GeneticTrainer::Train()
{
    //on each train reset the networks;
    for (auto& network : m_networks) {
        network.initWithRandomValues();
    }

    for (size_t i = 0; i < m_options.numEpisodes; i++) {
       // runEpisode();

        //selectNewNetworks();
        //crossover();
        //mutate();


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

double GeneticTrainer::runStep(const std::vector<int>& state, const GeneticNetwork& network)
{
    SnakeMove move=network.feedForward(state);
    GymEnv::StepResult result=m_env->Step(move);

    double reward = 0.0;

    switch (result.reward) {
        case -1: {
            reward = -1000.0;
            break;
        }
        case 0: {
            reward = 1.0;
            break;
        }
        case 1: {
            reward = 10.0;
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
    std::vector<int> state=m_env->GetState();

    for (size_t i = 0; i < m_options.maxNumSteps; i++) {

       double reward=runStep(state,network);

       network.updateFitness(reward);

    }

    
}

void GeneticTrainer::crossover()
{

}

void GeneticTrainer::selectNewNetworks()
{
    double fitnessSum=0.0;
    for (const auto& network : m_networks) {
        fitnessSum += network.getFitness();
    }

    m_networkProb[0] = m_networks[0].getFitness() / fitnessSum;

    for (size_t i = 1; i < m_options.numOfNetworks; i++) {
        double selectionProb = m_networks[i].getFitness() / fitnessSum;
        
        //ci = ci-1 + si; 
        m_networkProb[i] = m_networkProb[i - 1] + selectionProb;
    }

    std::vector<GeneticNetwork> newNetworks;
    newNetworks.resize(m_options.numOfNetworks);

    //generate numbers between (0,1]
    double selectionValue = 0.0;
    for (size_t i = 0; i < m_options.numOfNetworks; i++) {
        selectionValue = Utils::Math::randomDouble(0.00000001,1.0);

        GeneticNetwork selectedNetwork;

        for (size_t j = 0; j < m_options.numOfNetworks; j++) {
            if (selectionValue > m_networkProb[i]) {
                selectedNetwork = m_networks[i];
                break;
            }
        }

        newNetworks[i] = selectedNetwork;
    }
    m_networks = newNetworks;
}

void GeneticTrainer::mutate()
{

}


