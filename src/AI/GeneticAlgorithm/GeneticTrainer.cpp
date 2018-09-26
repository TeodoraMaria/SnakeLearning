#include "AI/GeneticAlgorithm/GeneticTrainer.h"
#include "AI/GeneticAlgorithm/GeneticJsonUtils.h"
#include "Utils/MathUtils.h"

#include <iostream>
#include <chrono>
#include <thread>


using namespace AI::GeneticAlgorithm;

GeneticTrainer::GeneticTrainer(Utils::NetworkSettings networkSettings,GeneticOptions options, GymEnv::SingleSnakeEnvBase* env):
    m_options(options),m_env(env)
{
    m_networks.resize(m_options.numOfNetworks);

    for (auto& network : m_networks) {
        network = GeneticNetwork(networkSettings);
    }  
}

IPlayerPtr GeneticTrainer::Train(TrainCallbacks callbacks)
{
    //on each train reset the networks;
    for (auto& network : m_networks) {
        network.initWeights();
    }

    for (size_t i = 0; i < m_options.numEpisodes; i++) {

        runEpisode(i);
        std::cout << "episode " << i;
        printFitnessInfo();

        selectNewNetworks();
        crossover();
        mutate();

        if (i < m_options.numEpisodes - 1) {
            resetFitness();
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

  


    //TODO: fix link;
    //SaveWeights(bestNetwork->getWeights(), "D:\\fac\\snake\\aux_files\\genetic\\TrainedGenetic.json");

    //return new GeneticBot(*bestNetwork, std::shared_ptr<GymEnv::StateObserver::IStateObserver>(m_env->GetObserver()->Clone()));

    // Sorry, no more Clone.
    // You should be able to create one from json.
   
    auto observer = const_cast<GymEnv::StateObserver::IStateObserver*>(m_env->GetObserver());

    return IPlayerPtr(new GeneticBot(*bestNetwork, std::shared_ptr<GymEnv::StateObserver::IStateObserver>(observer)));
}

void GeneticTrainer::runEpisode(size_t episode)
{
    size_t maxSteps;

    for (auto& network : m_networks) {     
        //run game for each bot;

        m_env->Reset();
        std::vector<double> state = m_env->GetState();

        for (size_t i = 0; i < m_options.maxNumSteps; i++) {

            double reward = runStep(state, network);
            if (reward == 1) {
                i = 0;
            }
            if (reward == -1000.0) {
                break;
            }
            
            //change to snake length
            network.updateFitness(reward);
            state = m_env->GetState();
        }
    }
    
    /*
    if (episode < m_options.numEpisodes-10) {
        return;
    }
    
    auto& bestNetwork=*std::max_element(m_networks.begin(),m_networks.end(),[](const auto& n1,const auto& n2){
        return n1.getFitness() < n2.getFitness();
    });

    m_env->Reset();
    std::vector<double> state = m_env->GetState();

    for (size_t i = 0; i < m_options.maxNumSteps; i++) {
        double reward = runStep(state, bestNetwork);
        if (Utils::Math::Approx(reward, 1.0, 0.1)) {
            i = 0;
        }

        if (Utils::Math::Approx(reward, -1000.0, 0.1)) {
            break;
        }
        m_env->Render();
        //TODO: remove this
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        //change to snake length
        //network.updateFitness(reward);
        state = m_env->GetState();
    }
    
    */
    
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
            //reward = -0.1;
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

void GeneticTrainer::crossover()
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
        
        /*
        //TODO: fix double comps
        if (selectionValue <= m_networks[0].getSelectionProb()) {
            selectedNetwork = m_networks[0];
        }

        for (size_t j = 0; j < m_options.numOfNetworks - 1; j++) {
            if (selectionValue > m_networks[j].getSelectionProb() && selectionValue <= m_networks[j + 1].getSelectionProb()) {
                selectedNetwork = m_networks[j + 1];
            }
        }
        */

        newNetworks[i] = selectedNetwork;
    }
    m_networks = newNetworks;
}


void GeneticTrainer::mutate()
{
    for (auto& network : m_networks) {
        network.mutateWeights(m_options.mutationProb);
    }
}

void GeneticTrainer::printFitnessInfo()
{
    size_t maxFitness=0;
    double totalFitness = 0;
    for (const auto& network: m_networks) {

        size_t networkFitness = std::cbrt(network.getFitness());

        if (maxFitness < networkFitness) {
            maxFitness = networkFitness;
        }
        totalFitness += networkFitness;
    }
    std::cout <<" max fitness: " <<maxFitness << " avg: "<<totalFitness/m_networks.size()<<std::endl;
}

void AI::GeneticAlgorithm::GeneticTrainer::resetFitness()
{
    for (auto& network : m_networks) {
        network.setFitness(0);
    }
}


