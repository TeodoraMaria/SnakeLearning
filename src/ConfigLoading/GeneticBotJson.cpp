#include "GeneticBotJson.h"
#include "AI/GeneticAlgorithm/GeneticNetwork.h"
#include "IObserverJson.h"
#include <vector>



using namespace Utils;


void AI::GeneticAlgorithm::from_json(const nlohmann::json & j, std::shared_ptr<AI::GeneticAlgorithm::GeneticBot>& player)
{
    NetworkSettings settings;
    if (j.find("input") != j.end()) {
        settings.m_inputs = j.at("input");
    }
    else {
        throw "input not found";
    }

    if (j.find("network_layers") != j.end()) {
        settings.m_hiddenLayersSizes= j.at("network_layers").get<std::vector<size_t>>();
    } else {
        throw "network layers not found";
    }

    GeneticNetwork network(settings);

    if (j.find("weights") != j.end()) {        
        network.setWeights(j.at("weights").get<std::vector<float>>());
    } else {
        throw "weights not found";
    }

    if (j.find("observer") != j.end()) {          
       player.reset(new GeneticBot(network, j.at("observer").get<std::shared_ptr<GymEnv::StateObserver::IStateObserver>>()));
    } else {
        throw "observer not found";
    }


}

