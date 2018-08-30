#pragma once

#include "AI/ITrainer.hpp"

namespace AI{ namespace QLearning
{
	class TabularTrainer : public AI::ITrainer
	{
	public:
		void Train() override;
	};
}}
