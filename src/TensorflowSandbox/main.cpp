#include "GameLogic/CellInterpreter/Basic3CellInterpreter.hpp"
#include "GymEnv/StateObserver/GridObserver.hpp"
#include "GameLogic/IPlayer.h"

#include <tensorflow/cc/ops/standard_ops.h>
#include <tensorflow/cc/client/client_session.h>
#include <tensorflow/cc/framework/gradients.h>
#include <tensorflow/core/framework/tensor.h>

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

	qoptions.numEpisodes = 1000;
	qoptions.randActionDecayFactor = 1.0 / (100);
	qoptions.learningRate = 0.0001;
	qoptions.maxRandActionChance = 10.0;
	qoptions.minRandActionChance = 0.05;
	qoptions.maxStepsWithoutFood = [&](int episode) -> size_t
	{
		return 70u + (double)episode / qoptions.numEpisodes * 150.0;
	};

	qoptions.foodReward = [](int episode) { return 1.0; };
	qoptions.dieReward = [&](int episode) { return -0.1; };//+ (double)episode / qoptions.numEpisodes * (-100.0); };
	qoptions.stepReward = [](int episode) { return 0; };

	qoptions.milsToSleepBetweenFrames = 25;
	qoptions.lastNGamesToRender = 10;

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

double ComputePartEqualityEps(const std::vector<double>& values, double percentage)
{
	const auto sum = std::accumulate(
		values.begin(),
		values.end(),
		0.0, // Init value.
		[](const auto val1, const auto val2)
		{
			return val1 + std::abs(val2);
		});
	
	return (sum / values.size()) * percentage;
}

int main()
{
	const auto learningRate = 0.01;

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
	
	auto layer1 = Add(scope, MatMul(scope, inputHolder, weights[0]), biasses[0]);
	
	auto& networkOutput = layer1;
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

	auto noise = qoptions.maxRandActionChance;
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
				actionIndx = AI::QLearning::Utils::PickActionAdditiveNoise(
					networkOutDbl,
					noise,
					merseneTwister,
					ComputePartEqualityEps(networkOutDbl, qoptions.actionQualityEps));
				
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
						qoptions.minRandActionChance = 0;
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
				
				/*
				** Training part.
				*/
				
				std::vector<std::vector<double>> observationsBatch = { observations };
				auto statesBatchTensor = MakeTensor(observationsBatch);
				std::vector<Tensor> q_s_a;
				TF_CHECK_OK(session.Run(
					{{ inputHolder, statesBatchTensor }},
					{ networkOutput },
					&q_s_a
				));

				std::vector<std::vector<double>> nextObservationsBatch = { nextObservations };
				auto nextStatesBatch = MakeTensor(nextObservationsBatch);
				std::vector<Tensor> q_s_a_d;
				TF_CHECK_OK(session.Run(
					{{ inputHolder, nextStatesBatch }},
					{ networkOutput },
					&q_s_a_d
				));
				
				const auto batchSize = 1;
				auto trainOutputBatch = Tensor(
					DataTypeToEnum<float>::v(),
					TensorShape{ (int)batchSize, (int)numOutputs });

				std::copy_n(
					q_s_a[0].matrix<float>().data(),
					batchSize * numOutputs,
					trainOutputBatch.matrix<float>().data()
				);

				for (auto batch_i = 0u; batch_i < batchSize; batch_i++)
				{
					float finalActionReward = reward;
					float bestNextQ = 0;
					if (!StateIsNone(nextObservations.data()))
					{
						const float* thisBatch_q_s_a_d = q_s_a_d[0].matrix<float>().data();
						bestNextQ = *std::max_element(thisBatch_q_s_a_d, thisBatch_q_s_a_d + numOutputs);
						finalActionReward += qoptions.qDiscountFactor * bestNextQ;
					}
					trainOutputBatch.matrix<float>()(actionIndx) = finalActionReward;
					
					if (g_debugging)
					{
						std::cout << "[BestNext]: " << bestNextQ << std::endl;
						std::cout << "[Final]: " << finalActionReward << std::endl;
						std::cout << "\033[36m " << trainOutputBatch.DebugString() << "\033[0m\n";
					}
				}

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
							{ inputHolder, statesBatchTensor },
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
		}
		
//		const auto discountedRewards = episodeMemory.GetDiscountedRewards(qoptions.qDiscountFactor);
//		const auto batchSize = episodeMemory.nbOfMemories;
//
//		auto statesBatch = MakeTensor(episodeMemory.states);
//		std::vector<Tensor> q_s_a;
//		TF_CHECK_OK(session.Run(
//			{{ inputHolder, statesBatch }},
//			{ networkOutput },
//			&q_s_a
//		));
//
//		auto nextStatesBatch = MakeTensor(episodeMemory.nextStates);
//		std::vector<Tensor> q_s_a_d;
//		TF_CHECK_OK(session.Run(
//			{{ inputHolder, nextStatesBatch }},
//			{ networkOutput },
//			&q_s_a_d
//		));
//
//		auto trainOutputBatch = Tensor(
//			DataTypeToEnum<float>::v(),
//			TensorShape{ (int)batchSize, (int)numOutputs });
//
//		std::copy_n(
//			q_s_a[0].matrix<float>().data(),
//			batchSize * numOutputs,
//			trainOutputBatch.matrix<float>().data()
//		);
//
//		for (auto batch_i = 0u; batch_i < batchSize; batch_i++)
//		{
//			const auto actionIndx = batch_i * numOutputs + episodeMemory.actions[batch_i];
//
////			trainOutputBatch.matrix<float>()(actionIndx) = discountedRewards[batch_i];
////
////			if (g_debugging)
////			{
////				std::cout
////					<< "Discounted: "
////					<< std::fixed << std::setprecision(2)
////					<< discountedRewards[batch_i] << std::endl;
////			}
//
//			float finalActionReward = episodeMemory.rewards[batch_i];
//			if (episodeMemory.nextStates[batch_i][0] != NoneFlag)
//			{
//				const float* thisBatch_q_s_a_d =
//					q_s_a_d[0].matrix<float>().data() + batch_i * numOutputs;
//
//				const auto bestNextQ = *std::max_element(thisBatch_q_s_a_d, thisBatch_q_s_a_d + numOutputs);
//				finalActionReward += qoptions.qDiscountFactor * bestNextQ;
//			}
//		}
//
//
//
//		{
//			auto stuffToBeComputed = std::vector<Output>();
//			for (auto& appliedGrad : appliedGrads)
//				stuffToBeComputed.push_back(appliedGrad);
//			stuffToBeComputed.push_back(networkOutput);
//			stuffToBeComputed.push_back(loss);
//
//			std::vector<Tensor> tfOutput;
//			TF_CHECK_OK(session.Run(
//				{
//					{ inputHolder, statesBatch },
//					{ targetQHolder, trainOutputBatch }
//				},
//				stuffToBeComputed,
//				&tfOutput
//			));
//
//			if (g_debugging)
//			{
//				std::cout
//					<< "------- [LOSS]:"
//					<< std::fixed << std::setprecision(2)
//					<< tfOutput.back().flat<float>()(0)
//					<< std::endl;
//			}
//		}
		
		printf(
			"----------------- End of episode: %d with a reward of \033[32m %.2f \033[0m. Noise: %.2f\n",
			episode,
			episodeReward,
			noise);
		noise = ::Utils::Math::Lerp(noise, qoptions.minRandActionChance, qoptions.randActionDecayFactor);
		
//		char c;
//		std::cin >> c;
	}
	return 0;
}

