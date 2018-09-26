#include "NeuralQAgent.hpp"

#include "GameLogic/CellInterpreter/Basic3CellInterpreter.hpp"
#include "GymEnv/StateObserver/GridObserver.hpp"
#include "GameLogic/IPlayer.h"

#include <tensorflow/cc/ops/standard_ops.h>
#include <tensorflow/cc/client/client_session.h>
#include <tensorflow/cc/framework/gradients.h>
#include <tensorflow/core/framework/tensor.h>
#include <tensorflow/core/public/session.h>
#include <tensorflow/core/protobuf/meta_graph.pb.h>

#include "GameLogic/Game.h"
#include "AI/QLearning/TabularTrainer.hpp"
#include "AI/QLearning/TabularQJsonUtils.h"
#include "AI/HardCoded/SingleBot.hpp"
#include "AI/GeneticAlgorithm/GeneticTrainer.h"
#include "AI/GeneticAlgorithm/GeneticOptions.h"
#include "AI/QLearning/MultisnakeTabularTrainer.hpp"
#include "AI/QLearning/State.h"
#include "AI/QLearning/ActionPickingUtils.h"
#include "AI/QLearning/QTabStudent.hpp"

#include "AI/QLearning/QTable.h"
#include "AI/QLearning/TabularQJsonUtils.h"
#include "AI/HardCoded/SingleBot.hpp"

#include "GymEnv/SnakeStudent.hpp"
#include "GymEnv/StateObserver/GridObserver.hpp"
#include "GymEnv/StateObserver/FullMapObserver.hpp"
#include "GymEnv/StateObserver/FoodDirectionDecorator.hpp"
#include "GymEnv/StateExtractor.hpp"
#include "GameView/OpenGLRenderer.h"

#include "GameLogic/CellInterpreter/Basic3CellInterpreter.hpp"
#include "GameLogic/CellInterpreter/WallFoodBody.hpp"
#include "GameLogic/CellInterpreter/WallFoodEnemy.hpp"

#include "Utils/MathUtils.h"
#include "Utils/PrintUtils.h"
#include "Utils/MatrixUtils.h"

#include "ConfigLoading/QTabStudentJson.h"
#include "ConfigLoading/IObserverJson.h"
#include "ConfigLoading/ICellInterpreterJson.h"
#include "AI/QLearning/TabularQJsonUtils.h"

#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
#include <json.hpp>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <unordered_map>
#include <json.hpp>

using namespace tensorflow;
using namespace tensorflow::ops;
using namespace AI::QLearning;

using json = nlohmann::json;

//static auto g_debugging = true;
static auto g_debugging = false;

NeuralQAgent::NeuralQAgent(
	std::shared_ptr<Scope> scope_in,
	std::shared_ptr<ClientSession> session_in,
	std::shared_ptr<GymEnv::StateObserver::IStateObserver> observer_in
) :
	observer(observer_in),
	scope(scope_in),
	session(session_in),
	inputHolder(*scope, tensorflow::DataType::DT_FLOAT),
	targetQHolder(*scope, tensorflow::DataType::DT_FLOAT)
{
	const int numInputs = NumInputs();
	const int numOutputs = NumOutputs();
	
	observationContainer.resize(numInputs);
	nextObservationContainer.resize(numInputs);
	
	/*
	** Neural network stuff.
	*/
	
	const auto learningRate = 0.001;
	
	// Weights.
	std::vector<int> layersShape{
	{
		numInputs,
		numInputs * 4,
		numInputs * 2,
		numOutputs,
	}};
	
	std::vector<Variable> weights;
	std::vector<Assign> weightsAssign;
	std::vector<Variable> biasses;
	std::vector<Assign> biassesAssign;
	
	for (auto i = 0u; i < layersShape.size() - 1; i++)
	{
		auto w = Variable(
			*scope,
			{ layersShape[i], layersShape[i + 1] }, // Shape
			DataType::DT_FLOAT);
		auto assignW = Assign(
			*scope, w,
			RandomNormal(
				*scope,
				{ layersShape[i], layersShape[i + 1] },
				DataType::DT_FLOAT)
		);
		
		auto b = Variable(
			*scope,
			{ 1, layersShape[i + 1] },
			DataType::DT_FLOAT);
		auto assignB = Assign(
			*scope, b,
			RandomNormal(*scope, { 1, layersShape[i + 1] }, DataType::DT_FLOAT));
	
		// Initi by running the graph once.
		TF_CHECK_OK(session->Run({ assignW, assignB }, nullptr));
		
		weights.emplace_back(w);
		biasses.emplace_back(b);
	}
	
	auto varIndx = 0u;
	auto layer1 = Tanh(*scope, Add(*scope, MatMul(*scope, inputHolder, weights[varIndx]), biasses[varIndx]));
	varIndx++;
	
	auto layer2 = Tanh(*scope, Add(*scope, MatMul(*scope, layer1, weights[varIndx]), biasses[varIndx]));
	varIndx++;
	
	auto layer3 = Add(*scope, MatMul(*scope, layer2, weights[varIndx]), biasses[varIndx]);
	varIndx++;
	networkOutput = layer3;
	
	loss = ReduceMean(*scope,
		Square(*scope, Sub(*scope, networkOutput, targetQHolder)),
		{ 0, 1 }
	);

	// Initialize the vector with variables.
	for (auto& w : weights)
		variables.push_back(w);
	for (auto& b : biasses)
		variables.push_back(b);
	
	// Apply Gradient Descent.
	std::vector<Output> gradOutputs;
	TF_CHECK_OK(AddSymbolicGradients(
		*scope,
		{ loss },
		variables,
		&gradOutputs));

	for (auto i = 0u; i < variables.size(); i++)
	{
		auto appliedGrad = ApplyGradientDescent(
			*scope,
			variables[i],
			Cast(*scope, learningRate, DataType::DT_FLOAT), { gradOutputs[i] });
		appliedGrads.emplace_back(appliedGrad);
	}

	// Other inits.
	observationTensor = Tensor(
		DataTypeToEnum<float>::v(),
		TensorShape{ 1, (int)observer->NbOfObservations() }
	);
}

int NeuralQAgent::NumInputs() const { return observer->NbOfObservations(); }
int NeuralQAgent::NumOutputs() const { return IPlayer::possibleMoves.size(); }

SnakeMove NeuralQAgent::GetNextAction(const GameState& gameState)
{
	const auto actionIndex = GetNextActionIndex(
		gameState,
		internalNoise,
		internalMerseneTwister,
		internalActionEqualityTreshold);
	
	return IPlayer::possibleMoves[actionIndex];
}

static json WeightsToJson(const ClientSession& session, const std::vector<Output>& vars)
{
	std::vector<Tensor> tfOuts;

	TF_CHECK_OK(session.Run(vars, &tfOuts));

	std::vector<std::vector<float>> varValues;
	for (auto& tfOut : tfOuts)
	{
		const float* flatData = tfOut.flat<float>().data();
		const auto totalNumParameters = tfOut.dim_size(0) * tfOut.dim_size(1);

		varValues.push_back(std::vector<float>(flatData, flatData + totalNumParameters));
	}

	return json(varValues);
}

static void JsonToWeights(
	ClientSession& session,
	Scope& scope,
	const json& data,
	std::vector<Output>& vars)
{
	auto varValues = data.get<std::vector<std::vector<float>>>();
	assert(varValues.size() == vars.size());

	std::vector<Tensor> tfOuts;
	TF_CHECK_OK(session.Run(vars, &tfOuts));

	for (auto i = 0u; i < vars.size(); i++)
	{
		const auto totalNumParameters = tfOuts[i].dim_size(0) * tfOuts[i].dim_size(1);
		assert(varValues[i].size() == totalNumParameters);

		auto paramsTensor = Tensor(
			DataTypeToEnum<float>::v(),
			TensorShape{ tfOuts[i].dim_size(0), tfOuts[i].dim_size(1) }
		);

		std::copy_n(varValues[i].begin(), totalNumParameters, paramsTensor.flat<float>().data());
		auto assign = Assign(scope, vars[i], paramsTensor, { 0, 1 });

		TF_CHECK_OK(session.Run({ assign }, nullptr));
	}
}

void AI::QLearning::to_json(nlohmann::json& j, NeuralQAgent* agent)
{
	j = nlohmann::json{
		{ "type", "NeuralQAgent" },
		{ "observer", json(agent->observer.get()) },
		{ "brains", WeightsToJson(*agent->session, agent->variables) }
	};
}

void AI::QLearning::from_json(const nlohmann::json& j, std::shared_ptr<NeuralQAgent> agent)
{
	auto scope = std::make_shared<Scope>(Scope::NewRootScope());
	auto session = std::make_shared<ClientSession>(*scope);
	agent.reset(new NeuralQAgent(
		scope,
		session,
		j.at("observer").get<std::shared_ptr<GymEnv::StateObserver::IStateObserver>>()
	));
	
	if (j.find("actionEqualityTreshold") != j.end())
		agent->internalActionEqualityTreshold = j.at("noise").get<double>();
	
	if (j.find("noise") != j.end())
		agent->internalNoise = j.at("noise").get<double>();
	
	// Brains.
	if (j.find("brainsFilePath") != j.end() && j.find("brains") != j.end())
		throw "Ambiguous: Both brains and brainsFilePath are provided.";
	
	if (j.find("brainsFilePath") != j.end())
	{
		agent->LoadWeights(j.at("brainsFilePath").get<std::string>());
	}
	else if (j.find("brains") != j.end())
	{
		JsonToWeights(*agent->session, *agent->scope, j, agent->variables);
	}
	else
	{
		// Do nothing
	}
}

void NeuralQAgent::LoadWeights(std::string filePath)
{
	std::ifstream stream;

	stream.open(filePath);
	if (!stream.is_open())
		throw std::runtime_error("Failed to open file");
	
	json j;
	stream >> j;
	
	JsonToWeights(*session, *scope, j, variables);
	stream.close();
}

void NeuralQAgent::SaveWeights(std::string filePath, bool compact)
{
	std::ofstream stream(filePath);

	if (!stream.is_open())
		throw std::runtime_error("Failed to open file");
	
	auto j = WeightsToJson(*session, variables);
	if (compact)
		stream << j;
	else
		stream << std::setw(2) << j;

	stream.close();
}

static double PositiveMean(const std::vector<double>& values)
{
	const auto sum = std::accumulate(
		values.begin(),
		values.end(),
		0.0, // Init value.
		[](const auto val1, const auto val2)
		{
			return val1 + std::abs(val2);
		});
	return sum / values.size();
}

int NeuralQAgent::GetNextActionIndex(
	const GameState& gmState,
	double noise,
	std::mt19937& merseneTwister,
	double actionEqualityTreshold)
{
	observer->Observe(observationContainer, gmState, GetSnakeNumber());
	std::copy_n(
		observationContainer.begin(),
		observer->NbOfObservations(),
		observationTensor.flat<float>().data());

	std::vector<Tensor> tfOutputs;
	TF_CHECK_OK(session->Run(
		{{ inputHolder, observationTensor }},
		{ networkOutput },
		&tfOutputs));
	const float* actionsQ = tfOutputs[0].flat<float>().data();
	auto networkOutDbl = std::vector<double>(actionsQ, actionsQ + NumOutputs());

	const auto positiveMean = PositiveMean(networkOutDbl);
	return AI::QLearning::Utils::PickActionAdditiveNoise(
		networkOutDbl,
		positiveMean * noise,
		merseneTwister,
		positiveMean * actionEqualityTreshold);
}

Tensor NeuralQAgent::PredictBatch(Tensor inputBatch)
{
	std::vector<Tensor> tfOuts;
	TF_CHECK_OK(session->Run(
		{{ inputHolder, inputBatch }},
		{ networkOutput },
		&tfOuts
	));
	
	return tfOuts[0];
}

void NeuralQAgent::Train(const Tensor& inputBatch, const Tensor& outputBatch)
{
	auto stuffToBeComputed = std::vector<Output>();
	for (auto& appliedGrad : appliedGrads)
		stuffToBeComputed.push_back(appliedGrad);
	stuffToBeComputed.push_back(networkOutput);
	stuffToBeComputed.push_back(loss);

	std::vector<Tensor> tfOutput;
	TF_CHECK_OK(session->Run(
		{
			{ inputHolder, inputBatch },
			{ targetQHolder, outputBatch }
		},
		stuffToBeComputed,
		&tfOutput
	));

	if (g_debugging)
	{
		std::cout
			<< "------- [LOSS]:"
			<< std::fixed << std::setprecision(2)
			<< tfOutput.back().flat<float>()(0)
			<< std::endl;
	}
}
