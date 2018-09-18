#pragma once

#include "GymEnv/StateObserver/IStateObserver.hpp"
#include <tensorflow/cc/framework/scope.h>

namespace AI { namespace QLearning
{
	class TfQStudent
	{
	public:
		TfQStudent(
			std::shared_ptr<GymEnv::StateObserver::IStateObserver> observer,
			double actionQulityCompareEps = 0.0,
			double learningRate = 0.01);
		
	private:
		std::shared_ptr<GymEnv::StateObserver::IStateObserver> m_observer;
		double m_actionQulityCompareEps;
		double m_learningRate;
		tensorflow::Scope m_tfScope;
	};
}}
