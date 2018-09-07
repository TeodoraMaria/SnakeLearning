#pragma once

#include "GymEnv/IActionPicker.hpp"
#include "QTabularBrains.hpp"
#include <random>

namespace AI { namespace QLearning
{
	class TabularActionPicker : public ActionPicking::IActionPicker
	{
	public:
		TabularActionPicker(
			size_t nbOfInterpretableBoardParts,
			double actionQEqualityEps,
			std::shared_ptr<QTabularBrains> brains);
	
		SnakeMove PickMove(
			const std::vector<double>& observations,
			double noise = 0) override;
	
	private:
		const size_t m_nbOfInterpretableBoardParts;
		const double m_actionQEqualityEps;
		std::shared_ptr<QTabularBrains> m_brains;
		std::mt19937 m_merseneTwister;
	};
}}
