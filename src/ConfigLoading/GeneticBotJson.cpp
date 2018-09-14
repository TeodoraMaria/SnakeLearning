#include "GeneticBotJson.h"
#include "AI/GeneticAlgorithm/GeneticNetwork.h"
#include "IObserverJson.h"


using namespace AI::GeneticAlgorithm;
using namespace Utils;


void AI::GeneticAlgorithm::from_json(const nlohmann::json & j, std::shared_ptr<GeneticBot>& player)
{
    NetworkSettings settings;
    if (j.find("input") != j.end()) {
        settings.m_inputs = j.at("input");
    }
    else {
        throw "input not found";
    }

    if (j.find("network_layers") != j.end()) {
        settings.m_hiddenLayersSizes = {j.at("network_layers")};
    } else {
        throw "network layers not found";
    }

    GeneticNetwork network(settings);

    if (j.find("weights") != j.end()) {        
        network.setWeights(j.at("weights"));
    } else {
        throw "weights not found";
    }

    if (j.find("observer") != j.end()) {          
        player.reset(new GeneticBot(network, j.at("observer")));
    } else {
        throw "observer not found";
    }


}
