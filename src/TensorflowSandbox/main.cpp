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

#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
#include <json.hpp>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <json.hpp>

using json = nlohmann::json;

//auto g_debugging = true;
auto g_debugging = false;

using namespace tensorflow;
using namespace tensorflow::ops;

using namespace AI::QLearning;
using namespace GameLogic;
using namespace GameLogic::CellInterpreter;
using namespace GymEnv::StateObserver;
using namespace GymEnv::Utils;

static const int NoneFlag = -42;

GameOptions GetGameOptions()
{
	auto gmOptions = GameOptions();
	gmOptions.boardLength = 25;
	gmOptions.boardWidth = 25;
	gmOptions.numFoods = 10;
	gmOptions.initialSnakeSize = 3;
	gmOptions.gameRenderer = new GameView::TermRenderer();
//		gmOptions.gameRenderer = new GameView::OpenGLRenderer(
//			500, 500,
//			gmOptions.boardLength, gmOptions.boardWidth);

	return gmOptions;
}

QOptions GetQOptions()
{
	auto qoptions = AI::QLearning::QOptions();
	qoptions.maxNumSteps = [&](int episode)
	{
		return 50000;
	};
	qoptions.qDiscountFactor = 0.95;
	qoptions.actionQualityEps = 0.05;

	qoptions.numEpisodes = 500;
	qoptions.learningRate = 0.0001;
	
	// Percentage of mean.
	qoptions.maxNoise = 10.0;
	qoptions.minNoise = 0.02;
	qoptions.noiseDecayFactor = 1.0 / (qoptions.numEpisodes * 0.1);
	
	qoptions.maxStepsWithoutFood = [&](int episode) -> size_t
	{
		return 70u + (double)episode / qoptions.numEpisodes * 150.0;
	};

	qoptions.foodReward = [](int episode)	{ return  1.0; };

//	qoptions.dieReward = [&](int episode)	{ return -episode * episode / std::pow(qoptions.numEpisodes, 1.8); };
	qoptions.dieReward = [&](int episode)	{ return 0; };
	
//	qoptions.stepReward = [](int episode)	{ return -0.001; };
	qoptions.stepReward = [](int episode)	{ return 0; };

	qoptions.milsToSleepBetweenFrames = 25;
	qoptions.lastNGamesToRender = 2;

	return qoptions;
}

int ProbabilisticallyPickAction(
	const std::vector<float>& probabilities,
	std::mt19937& merseneTwister)
{
	auto chanceDistrib = std::uniform_real_distribution<float>(0, 1.0);
	auto randomNb = chanceDistrib(merseneTwister);
	
	for (auto i = 0u; i < probabilities.size(); i++)
	{
		if (randomNb <= probabilities[i])
			return i;
		
		randomNb -= probabilities[i];
	}
	return probabilities.size() - 1;
}

struct TrainMemory
{
	std::vector<std::vector<double>> states;
	std::vector<int> actions;
	std::vector<double> rewards;
	std::vector<std::vector<double>> nextStates;
	
	int nbOfMemories = 0;
	
	void RegisterMemory(
		const std::vector<double>& state,
		int action,
		double reward,
		const std::vector<double>& nextState)
	{
		states.push_back(state);
		actions.push_back(action);
		rewards.push_back(reward);
		nextStates.push_back(nextState);
		
		nbOfMemories++;
	}
	
	std::vector<float> GetDiscountedRewards(double qDiscountFactor)
	{
		std::vector<float> discountedRewards;
		discountedRewards.reserve(rewards.size());
		
		float lastReward = 0;
		for (auto it = rewards.rbegin(); it != rewards.rend(); it++)
		{
			lastReward = lastReward * qDiscountFactor + (*it);
			discountedRewards.push_back(lastReward);
		}
		std::reverse(discountedRewards.begin(), discountedRewards.end());
		
		return discountedRewards;
	}
};

Tensor MakeTensor(std::vector<std::vector<double>>& data)
{
	auto result = Tensor(
		DataTypeToEnum<float>::v(),
		TensorShape{ (int)data.size(), (int)data.front().size() });

	for (auto gi = 0u; gi < data.size(); gi++)
	{
		for (auto i = 0u; i < data.front().size(); i++)
		{
			const auto indx = gi * data.front().size() + i;
			result.matrix<float>()(indx) = data[gi][i];
		}
	}
	
	return result;
}

bool StateIsNone(const double* state)
{
	return ::Utils::Math::Approx(state[0], NoneFlag, 0.1);
}

double PositiveMean(const std::vector<double>& values)
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

double ComputePartEqualityEps(const std::vector<double>& values, double percentage)
{
	return PositiveMean(values) * percentage;
}

json WeightsToJson(const ClientSession& session, const std::vector<Output>& vars)
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

void JsonToWeights(
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

void TrySaveWeights(std::string fileName, const ClientSession& session, const std::vector<Output>& vars)
{
	std::ofstream stream(fileName);
	
	if (!stream.is_open())
	{
		std::cout << "[Warning]: " << "Failed to open " << fileName << std::endl;
		return;
	}
	
	stream << std::setw(2) << WeightsToJson(session, vars);
	stream.close();
}

void TryLoadWeights(
	std::string fileName,
	ClientSession& session,
	Scope& scope,
	std::vector<Output>& vars)
{
	std::ifstream stream;
	
	stream.open(fileName);
	if (!stream.is_open())
	{
		std::cout << "[Warning]: " << "Failed to load TF network from " << fileName << std::endl;
		return;
	}
	
	json j;
	stream >> j;
	
	JsonToWeights(session, scope, j, vars);
	stream.close();
	
	std::cout << "Loaded: " << std::endl << std::setw(2) << j << std::endl;
}

int main()
{
	const auto learningRate = 0.0001;

	auto cellInterpreter = std::make_shared<CellInterpreter::Basic3CellInterpreter>();
	auto observer = std::make_shared<GridObserver>(cellInterpreter, 5, 5);
//	auto observer = std::make_shared<FullMapObserver>(cellInterpreter, 25 * 25);
	
	const int numInputs = observer->NbOfObservations();
	const int numOutputs = IPlayer::possibleMoves.size();
	
	auto scope = Scope::NewRootScope();

	auto inputHolder = Placeholder(scope, tensorflow::DataType::DT_FLOAT);
	
	// Weights.
	std::vector<int> layersShape{
	{
		numInputs,
		numInputs * 2,
//		numInputs * 2,
		numOutputs,
	}};
	
	std::vector<Variable> weights;
	std::vector<Assign> weightsAssign;
	std::vector<Variable> biasses;
	std::vector<Assign> biassesAssign;
	
	for (auto i = 0u; i < layersShape.size() - 1; i++)
	{
		auto w = Variable(scope, { layersShape[i], layersShape[i + 1] }, DataType::DT_FLOAT);
		auto assignW = Assign(scope, w, RandomNormal(scope, { layersShape[i], layersShape[i + 1] }, DataType::DT_FLOAT));
		
		auto b = Variable(scope, { 1, layersShape[i + 1] }, DataType::DT_FLOAT);
		auto assignB = Assign(scope, b, RandomNormal(scope, { 1, layersShape[i + 1] }, DataType::DT_FLOAT));
		
		weights.emplace_back(w);
		weightsAssign.emplace_back(assignW);
		
		biasses.emplace_back(b);
		biassesAssign.emplace_back(assignB);
	}
	
	auto varIndx = 0u;
	auto layer1 = Elu(scope, Add(scope, MatMul(scope, inputHolder, weights[varIndx]), biasses[varIndx])); varIndx++;
//	auto layer2 = Elu(scope, Add(scope, MatMul(scope, layer1, weights[varIndx]), biasses[varIndx])); varIndx++;
	auto layer2= Add(scope, MatMul(scope, layer1, weights[varIndx]), biasses[varIndx]); varIndx++;
	auto& networkOutput = layer2;
	
	auto targetQHolder = Placeholder(scope, DataType::DT_FLOAT);
	
	auto loss = ReduceMean(scope,
		Square(scope, Sub(scope, networkOutput, targetQHolder)),
		{ 0, 1 }
	);
	
	/*
	** Backpropagation
	*/
	
	std::vector<Output> gradOutputs;
	auto variables = std::vector<Output>();
	for (auto& w : weights)
		variables.push_back(w);
	for (auto& b : biasses)
		variables.push_back(b);
	
	TF_CHECK_OK(AddSymbolicGradients(
		scope,
		{ loss },
		variables,
		&gradOutputs));

	std::vector<Output> appliedGrads;
	for (auto i = 0u; i < variables.size(); i++)
	{
		auto appliedGrad = ApplyGradientDescent(
			scope,
			variables[i],
			Cast(scope, learningRate, DataType::DT_FLOAT), { gradOutputs[i] });
		appliedGrads.emplace_back(appliedGrad);
	}

	ClientSession session(scope);

	// Init the weights and biasses by running the assigns nodes once.
	std::vector<Output> assignedVars;
	for (auto& w : weightsAssign)
		assignedVars.push_back(w);
	for (auto& b : biassesAssign)
		assignedVars.push_back(b);
	TF_CHECK_OK(session.Run(assignedVars, nullptr));

	const std::string tfNetworkJsonFileName = "tfNetwork.json";
	TryLoadWeights(tfNetworkJsonFileName, session, scope, variables);
	
	std::vector<Tensor> testWWW;
	session.Run({ weights[0] }, &testWWW);
	
	std::cout << testWWW[0].DebugString() << std::endl;
	
	/*
	** -------------------------------------------- End of network declaration -
	*/

	auto gmOptions = GetGameOptions();
	auto qoptions = GetQOptions();

	auto agents = std::vector<std::shared_ptr<GymEnv::SnakeStudent>>(
	{
		std::make_shared<GymEnv::SnakeStudent>()
	});

	auto players = std::vector<std::shared_ptr<IPlayer>>();
	for (auto agent : agents)
		players.push_back(std::static_pointer_cast<IPlayer>(agent));

	auto game = Game(gmOptions, players);
	
	auto merseneTwister = std::mt19937(std::random_device()());
	auto chanceDistrib = std::uniform_real_distribution<double>(0.0, 1.0);
	auto actionDistrib = std::uniform_int_distribution<>(0, IPlayer::possibleMoves.size());
	
	auto observations = std::vector<double>(observer->NbOfObservations());
	auto nextObservations = std::vector<double>(observer->NbOfObservations());
	auto requiredOutput = std::vector<float>(IPlayer::possibleMoves.size());
	auto startedToRender = false;

	auto noise = qoptions.maxNoise;
	auto networkOutCpy = std::vector<float>(numOutputs);

	if (g_debugging)
		std::cout << std::endl << std::endl;

	for (auto episode = 0u; episode < qoptions.numEpisodes; episode++)
	{
		game.InitGame();
		const auto maxNbOfSteps = qoptions.maxNumSteps(episode);
		const auto maxStepsWithoutFood = qoptions.maxStepsWithoutFood(episode);

		auto stepsWithoutFood = 0u;
		auto episodeReward = 0.0;
		
		auto episodeMemory = TrainMemory();
		for (auto step = 0u; step < maxNbOfSteps; step++)
		{
			if (game.EveryoneIsDead())
				break;
			
			for (auto& agent : agents)
			{
				const auto snakeId = agent->GetSnakeNumber();
				const auto gmState = game.GetGameState();
				
				if (!gmState.GetSnake(snakeId).IsAlive())
					continue;
				
				if (stepsWithoutFood >= maxStepsWithoutFood)
				{
					std::cout << snakeId << " starved to death. ----- " << std::endl;
					game.ForcefullyKillPlayer(snakeId);
					continue;
				}
				
				observer->Observe(observations, gmState, snakeId);
				auto observationTensor = Tensor(
					DataTypeToEnum<float>::v(),
					TensorShape{ 1, (int)observer->NbOfObservations() }
				);
				std::copy_n(
					observations.begin(),
					observer->NbOfObservations(),
					observationTensor.flat<float>().data());
				
				std::vector<Tensor> tfOutputs;
				TF_CHECK_OK(session.Run(
					{{ inputHolder, observationTensor }},
					{ networkOutput },
					&tfOutputs));
				const float* actionsQ = tfOutputs[0].flat<float>().data();

				int actionIndx = 0;
				auto networkOutDbl = std::vector<double>(actionsQ, actionsQ + numOutputs);
				
				const auto positiveMean = PositiveMean(networkOutDbl);
				actionIndx = AI::QLearning::Utils::PickActionAdditiveNoise(
					networkOutDbl,
					positiveMean * noise,
					merseneTwister,
					positiveMean * qoptions.actionQualityEps);
				
				if (g_debugging)
				{
					std::cout << "\033[33m[NetworkOutput]:" << tfOutputs[0].DebugString() << "\033[0m" << std::endl;
				}

				if (episode >= qoptions.numEpisodes - qoptions.lastNGamesToRender)
				{
					if (!startedToRender)
					{
						startedToRender = true;
						std::system("play -q -n synth 1 sin 880");
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
						qoptions.minNoise = 0;
					}
					gmOptions.gameRenderer->Render(game.GetGameState());
					if (qoptions.milsToSleepBetweenFrames != 0)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(qoptions.milsToSleepBetweenFrames));
					}
				}
				
				const auto action = IPlayer::possibleMoves[actionIndx];
				const auto rawReward = game.MoveSnake(snakeId, action);

				auto reward = 0.0;
				if (rawReward < 0)
				{
					reward += qoptions.dieReward(episode);
				}
				else if (rawReward > 0)
				{
					reward += qoptions.foodReward(episode);
					stepsWithoutFood = 0;
				}
				else
				{
					reward += qoptions.stepReward(episode);
					stepsWithoutFood++;
				}

				if (g_debugging)
					std::cout << "\033[34mReward: " << reward << "\033[0m" << std::endl;

				episodeReward += reward;
				const auto isDone = (rawReward < 0);
				
				if (isDone)
				{
					nextObservations[0] = NoneFlag;
				}
				else
				{
					const auto gmStatePrim = game.GetGameState();
					observer->Observe(nextObservations, gmStatePrim, snakeId);
				}
				episodeMemory.RegisterMemory(observations, actionIndx, reward, nextObservations);
				game.RestockFood();
			}
		}
		
		const auto discountedRewards = episodeMemory.GetDiscountedRewards(qoptions.qDiscountFactor);
		for (int batch_i = episodeMemory.nbOfMemories - 1; batch_i >= 0; batch_i--)
		{
			const auto batchSize = 1;

			std::vector<std::vector<double>> observationsBatch = { episodeMemory.states[batch_i] };
			auto statesBatch = MakeTensor(observationsBatch);
			std::vector<Tensor> q_s_a;
			TF_CHECK_OK(session.Run(
				{{ inputHolder, statesBatch }},
				{ networkOutput },
				&q_s_a
			));

			std::vector<std::vector<double>> nextObservationsBatch = { episodeMemory.nextStates[batch_i] };
			auto nextStatesBatch = MakeTensor(nextObservationsBatch);
			std::vector<Tensor> q_s_a_d;
			TF_CHECK_OK(session.Run(
				{{ inputHolder, nextStatesBatch }},
				{ networkOutput },
				&q_s_a_d
			));

			auto trainOutputBatch = Tensor(
				DataTypeToEnum<float>::v(),
				TensorShape{ (int)batchSize, (int)numOutputs });

			std::copy_n(
				q_s_a[0].matrix<float>().data(),
				batchSize * numOutputs,
				trainOutputBatch.matrix<float>().data()
			);

			const auto actionIndx = episodeMemory.actions[batch_i];
			float finalActionReward = discountedRewards[batch_i];
//			float finalActionReward = episodeMemory.rewards[batch_i];

			if (!StateIsNone(episodeMemory.nextStates[batch_i].data()))
			{
				const float* thisBatch_q_s_a_d = q_s_a_d[0].matrix<float>().data();
				const auto bestNextQ = *std::max_element(thisBatch_q_s_a_d, thisBatch_q_s_a_d + numOutputs);
				finalActionReward += qoptions.qDiscountFactor * bestNextQ;
			}
			trainOutputBatch.matrix<float>()(actionIndx) = finalActionReward;

			// Train.
			{
				auto stuffToBeComputed = std::vector<Output>();
				for (auto& appliedGrad : appliedGrads)
					stuffToBeComputed.push_back(appliedGrad);
				stuffToBeComputed.push_back(networkOutput);
				stuffToBeComputed.push_back(loss);

				std::vector<Tensor> tfOutput;
				TF_CHECK_OK(session.Run(
					{
						{ inputHolder, statesBatch },
						{ targetQHolder, trainOutputBatch }
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
		}
		
		printf(
			"----------------- End of episode: %d with a reward of\033[32m %.2f\033[0m. Noise: %.2f. Die reward: %.2f\n",
			episode,
			episodeReward,
			noise,
			qoptions.dieReward(episode));
		noise = ::Utils::Math::Lerp(noise, qoptions.minNoise, qoptions.noiseDecayFactor);
		
//		char c;
//		std::cin >> c;
	}
	
	TrySaveWeights(tfNetworkJsonFileName, session, variables);
	
	return 0;
}

