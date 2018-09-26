#pragma once

#include "GameLogic/IPlayer.h"
#include "GymEnv/StateObserver/IStateObserver.hpp"

#include <tensorflow/cc/client/client_session.h>
#include <tensorflow/cc/framework/gradients.h>
#include <tensorflow/core/framework/tensor.h>
#include <tensorflow/cc/ops/array_ops.h>
#include <json_fwd.hpp>

#include <random>
#include <memory>

namespace AI { namespace QLearning
{
	using namespace tensorflow;
	
	class NeuralQAgent : public IPlayer
	{
	public:
		NeuralQAgent(
			std::shared_ptr<Scope> scope_in,
			std::shared_ptr<ClientSession> session_in,
			std::shared_ptr<GymEnv::StateObserver::IStateObserver> observer_in
		);
		
		int NumInputs() const;
		int NumOutputs() const;

		SnakeMove GetNextAction(const GameState& gameState) override;

		friend void to_json(nlohmann::json& j, NeuralQAgent* agent);
		friend void from_json(const nlohmann::json& j, std::shared_ptr<NeuralQAgent>& agent);

		void LoadWeights(std::string filePath);
		void SaveWeights(std::string filePath, bool compact = false);

		int GetNextActionIndex(
			const GameState& gmState,
			double noise,
			std::mt19937& merseneTwister,
			double actionEqualityTreshold);

		Tensor PredictBatch(Tensor inputBatch);

		void Train(const Tensor& inputBatch, const Tensor& outputBatch);

		std::shared_ptr<GymEnv::StateObserver::IStateObserver> observer;
		std::vector<double> observationContainer;
		std::vector<double> nextObservationContainer;

		// Things used only for performance (after training).
		double internalNoise = 0.01;
		double internalActionEqualityTreshold = 0.01;
		std::mt19937 internalMerseneTwister = std::mt19937(std::random_device()());

	private:
		Tensor observationTensor;

		std::shared_ptr<Scope> scope;
		std::shared_ptr<ClientSession> session;

		ops::Placeholder inputHolder;
		ops::Placeholder targetQHolder;
		Output networkOutput;
		Output loss;

		std::vector<Output> variables;
		std::vector<Output> appliedGrads;
	};
	
}}
