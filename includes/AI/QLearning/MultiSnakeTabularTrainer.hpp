#pragma once

#include "QTable.h"
#include "QOptions.h"
#include "TabularAgent.hpp"

#include "AI/ITrainer.hpp"
#include "GymEnv/MultiSnakeEnv.hpp"

#include <cstddef>
#include <random>
#include <unordered_map>

namespace AI { namespace QLearning
{
	class MultiSnakeTabularTrainer : public AI::ITrainer
	{
	public:
		MultiSnakeTabularTrainer(
			QOptions& qoptions,
			std::vector<std::shared_ptr<TabularAgent>> agents,
			std::shared_ptr<GymEnv::MultiSnakeEnv> env);
		
		IPlayer* Train() override;
		
	private:
		struct TrainSession;
	
		void RunEpisode(TrainSession& trainSession);
		double ComputeStepReward(int rawReward, int episode) const;
		
		QOptions& m_qoptions;
		std::vector<std::shared_ptr<TabularAgent>> m_agents;
		std::shared_ptr<GymEnv::MultiSnakeEnv> m_env;
	};
	
	struct MultiSnakeTabularTrainer::TrainSession
	{
		unsigned int episode;
		double noise;
	};
}}
