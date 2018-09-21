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


using namespace tensorflow;
using namespace tensorflow::ops;

using namespace AI::QLearning;
using namespace GameLogic;
using namespace GameLogic::CellInterpreter;
using namespace GymEnv::StateObserver;
using namespace GymEnv::Utils;

int main()
{
	const auto learningRate = 0.01;

	auto cellInterpreter = std::make_shared<CellInterpreter::Basic3CellInterpreter>();
	auto observer = std::make_shared<GridObserver>(cellInterpreter, 3, 3);
	
	auto scope = Scope::NewRootScope();

	auto input = Placeholder(scope, tensorflow::DataType::DT_FLOAT);
	auto output = Placeholder(scope, tensorflow::DataType::DT_FLOAT);
	
	auto inputTensor = Tensor(DataTypeToEnum<float>::v(), TensorShape{ 1, (int)observer->NbOfObservations() });
	auto outputTensor = Tensor(DataTypeToEnum<float>::v(), TensorShape{ 1, (int)IPlayer::possibleMoves.size() });
	
	const int numInputs = observer->NbOfObservations();
	const int numOutputs = IPlayer::possibleMoves.size();
	
	const auto layersSizes = std::array<int, 2>{{ numInputs * 2, (numInputs + numOutputs) * 2 }};
	
	// Weights.
	const int shape1[] = { numInputs, numOutputs };
	auto w1 = Variable(scope, { shape1[0], shape1[1] }, DataType::DT_FLOAT);
	auto assignW1 = Assign(scope, w1, RandomNormal(scope, { shape1[0], shape1[1] }, DataType::DT_FLOAT));
	
//	const int shape2[] = { layersSizes[0], layersSizes[1] };
//	auto w2 = Variable(scope, { shape2[0], shape2[1] }, DataType::DT_FLOAT);
//	auto assignW2 = Assign(scope, w2, RandomNormal(scope, { shape2[0], shape2[1] }, DataType::DT_FLOAT));
//
//	const int shape3[] = { layersSizes[1], numOutputs };
//	auto w3 = Variable(scope, { shape3[0], shape3[1] }, DataType::DT_FLOAT);
//	auto assignW3 = Assign(scope, w3, RandomNormal(scope, { shape3[0], shape3[1] }, DataType::DT_FLOAT));
	
	// Bias.
	auto b1 = Variable(scope, { 1, shape1[1] }, DataType::DT_FLOAT);
	auto assignB1 = Assign(scope, b1, RandomNormal(scope, { 1, shape1[1] }, DataType::DT_FLOAT));
	
//	auto b2 = Variable(scope, { 1, shape2[1] }, DataType::DT_FLOAT);
//	auto assignB2 = Assign(scope, b2, RandomNormal(scope, { 1, shape2[1] }, DataType::DT_FLOAT));
//
//	auto b3 = Variable(scope, { 1, shape3[1] }, DataType::DT_FLOAT);
//	auto assignB3 = Assign(scope, b3, RandomNormal(scope, { 1, shape3[1] }, DataType::DT_FLOAT));
	
	// Layers.
	auto layer1 = Add(scope, MatMul(scope, input, w1), b1);
//	auto layer2 = Tanh(scope, Add(scope, MatMul(scope, layer1, w2), b2));
//	auto layer3 = Tanh(scope, Add(scope, MatMul(scope, layer2, w3), b3));
//	auto layer3 = Add(scope, MatMul(scope, layer2, w3), b3);
	
	auto& lastLayer = layer1;
	
	auto predict = ArgMax(scope, lastLayer, 1);
	auto nextQ = Placeholder(scope, DataType::DT_FLOAT);
	auto nextQTensor = Tensor(DataTypeToEnum<float>::v(), TensorShape{ 1, (int)IPlayer::possibleMoves.size() });
	
	auto reqularization = AddN(
		scope,
		std::initializer_list<Input>{ L2Loss(scope, w1) }// L2Loss(scope, w2), L2Loss(scope, w3)}
	);
//	auto loss = Add(
//		scope,
//		ReduceMean(scope, Square(scope, Sub(scope, layer3, output)), { 0, 1 }),
//		Mul(scope, Cast(scope, learningRate, DataType::DT_FLOAT), reqularization)
//	);
	
	auto loss = ReduceMean(scope, Square(scope, Sub(scope, lastLayer, nextQ)), { 0, 1 });
	
	/*
	** Backpropagation.
	*/
	
	std::vector<Output> gradOutputs;
	TF_CHECK_OK(AddSymbolicGradients(
		scope,
		{ loss },
		{
			w1, // w2, w3,
			b1, // b2, b3
		},
		&gradOutputs));
	
	auto applyW1 = ApplyGradientDescent(scope, w1, Cast(scope, learningRate, DataType::DT_FLOAT), { gradOutputs[0] });
//	auto applyW2 = ApplyGradientDescent(scope, w2, Cast(scope, learningRate, DataType::DT_FLOAT), { gradOutputs[1] });
//	auto applyW3 = ApplyGradientDescent(scope, w3, Cast(scope, learningRate, DataType::DT_FLOAT), { gradOutputs[2] });

	auto applyB1 = ApplyGradientDescent(scope, b1, Cast(scope, learningRate, DataType::DT_FLOAT), { gradOutputs[1] });
//	auto applyB2 = ApplyGradientDescent(scope, b2, Cast(scope, learningRate, DataType::DT_FLOAT), { gradOutputs[4] });
//	auto applyB3 = ApplyGradientDescent(scope, b3, Cast(scope, learningRate, DataType::DT_FLOAT), { gradOutputs[5] });

	ClientSession session(scope);
	std::vector<Tensor> tfOutputs;

	// Init the weights and biases by running the assigns nodes once.
	TF_CHECK_OK(session.Run(
		{
			assignW1, // assignW2, assignW3,
			assignB1, // assignB2, assignB3
		},
		nullptr
	));

	/*
	** -------------------------------------------------------------------------
	*/

	auto gmOptions = GameOptions();
	{
		gmOptions.boardLength = 25;
		gmOptions.boardWidth = 25;
		gmOptions.numFoods = 10;
		gmOptions.initialSnakeSize = 3;
		gmOptions.gameRenderer = new GameView::TermRenderer();
//		gmOptions.gameRenderer = new GameView::OpenGLRenderer(
//			500, 500,
//			gmOptions.boardLength, gmOptions.boardWidth);
	}

	auto qoptions = AI::QLearning::QOptions();
	{
		qoptions.maxNumSteps = [&](int episode)
		{
			return 50000;// + (double)episode / qoptions.numEpisodes * 3000;
		};
		qoptions.qDiscountFactor = 0.9;
		qoptions.actionQualityEps = 0.005;

		qoptions.numEpisodes = 1000;
		qoptions.randActionDecayFactor = 1.0 / (50);
		qoptions.learningRate = 0.1;
		qoptions.maxRandActionChance = 1.0;
		qoptions.minRandActionChance = 0.001;
		qoptions.maxStepsWithoutFood = [&](int episode) -> size_t
		{
			return 150u + (double)episode / qoptions.numEpisodes * 300.0;
		};

		qoptions.foodReward = [](int episode) { return 1.0; };
		qoptions.dieReward = [&](int episode) { return 0; };//+ (double)episode / qoptions.numEpisodes * (-100.0); };
		qoptions.stepReward = [](int episode) { return 0; };

		qoptions.milsToSleepBetweenFrames = 25;
		qoptions.lastNGamesToRender = 5;
	}

	auto agents = std::vector<std::shared_ptr<GymEnv::SnakeStudent>>(
	{
		std::make_shared<GymEnv::SnakeStudent>()
	});

	auto players = std::vector<std::shared_ptr<IPlayer>>();
	for (auto agent : agents)
		players.push_back(std::static_pointer_cast<IPlayer>(agent));

	auto game = Game(gmOptions, players);
	auto observationContainer = std::vector<double>(observer->NbOfObservations());
	auto nextObservationContainer = std::vector<double>(observer->NbOfObservations());
	auto requiredOutput = std::vector<float>(IPlayer::possibleMoves.size());
	auto startedToRender = false;
	
	auto noise = qoptions.maxRandActionChance;
	auto merseneTwister = std::mt19937(std::random_device()());
	auto chanceDistrib = std::uniform_real_distribution<double>(0.0, 1.0);
	auto actionDistrib = std::uniform_int_distribution<>(0, IPlayer::possibleMoves.size());

	for (auto episode = 0u; episode < qoptions.numEpisodes; episode++)
	{
		game.InitGame();
		const auto maxNbOfSteps = qoptions.maxNumSteps(episode);
		const auto maxStepsWithoutFood = qoptions.maxStepsWithoutFood(episode);
		
		auto stepsWithoutFood = 0u;
		auto episodeReward = 0.0;
		for (auto step = 0u; step < maxNbOfSteps; step++)
		{
			if (game.EveryoneIsDead())
				break;

			for (auto& agent : agents)
			{
				auto gmState = game.GetGameState();

				const auto snakeId = agent->GetSnakeNumber();
				if (!gmState.GetSnake(snakeId).IsAlive())
					continue;
				
				if (stepsWithoutFood >= maxStepsWithoutFood)
				{
					std::cout << snakeId << " starved to death. ----- " << std::endl;
					game.ForcefullyKillPlayer(snakeId);
					continue;
				}
				
				observer->Observe(observationContainer, gmState, snakeId);
				std::copy_n(observationContainer.begin(), observer->NbOfObservations(), inputTensor.flat<float>().data());
				TF_CHECK_OK(session.Run({{ input, inputTensor }}, { lastLayer }, &tfOutputs));
				
				std::vector<Tensor> tfOutputsPrint;
				TF_CHECK_OK(session.Run({}, { w1 }, &tfOutputsPrint));
				std::cout << tfOutputsPrint[0].DebugString() << std::endl;
				
				if (episode >= qoptions.numEpisodes - qoptions.lastNGamesToRender)
				{
					if (!startedToRender)
					{
						startedToRender = true;
						std::system("play -q -n synth 1 sin 880");
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					}
					gmOptions.gameRenderer->Render(game.GetGameState());
					if (qoptions.milsToSleepBetweenFrames != 0)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(qoptions.milsToSleepBetweenFrames));
					}
				}

				unsigned int bestMoveIndx;
				const auto networkOutput = tfOutputs[0].flat<float>().data();
				
				if (chanceDistrib(merseneTwister) < noise)
					bestMoveIndx = actionDistrib(merseneTwister);
				else
				{
					const auto bestMoveQ = std::max_element(networkOutput, networkOutput + IPlayer::possibleMoves.size());
					bestMoveIndx = std::distance(networkOutput, bestMoveQ);
				}
				
				const auto rawReward = game.MoveSnake(
					snakeId,
					IPlayer::possibleMoves[bestMoveIndx]);
				
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
				
				episodeReward += reward;
				
				double bestNextQ = 0;
				const auto isDone = (rawReward < 0);
				if (!isDone)
				{
					const auto gmState = game.GetGameState();
					observer->Observe(nextObservationContainer, gmState, snakeId);
					std::copy_n(nextObservationContainer.begin(), observer->NbOfObservations(), inputTensor.flat<float>().data());
					TF_CHECK_OK(session.Run({{ input, inputTensor }}, { lastLayer, predict }, &tfOutputs));
					
					const auto networkOutput = tfOutputs[0].flat<float>().data();
					const auto bestMoveQ = std::max_element(networkOutput, networkOutput + IPlayer::possibleMoves.size());
					std::cout
						<< tfOutputs[1].DebugString() << " || "
						<< tfOutputs[0].DebugString() << " || "
						<< std::distance(networkOutput, bestMoveQ) << std::endl;
					bestNextQ = *bestMoveQ;
				}
				
				std::copy_n(observationContainer.begin(), observer->NbOfObservations(), inputTensor.flat<float>().data());
				
//				std::copy_n(networkOutput, IPlayer::possibleMoves.size(), requiredOutput.begin());
				std::fill(requiredOutput.begin(), requiredOutput.end(), 0);
				requiredOutput[bestMoveIndx] = reward + qoptions.qDiscountFactor * bestNextQ;
				
				std::copy_n(requiredOutput.begin(), IPlayer::possibleMoves.size(), nextQTensor.flat<float>().data());
				std::copy_n(requiredOutput.begin(), IPlayer::possibleMoves.size(), outputTensor.flat<float>().data());
				
//				TF_CHECK_OK(session.Run(
//					{{ input, inputTensor }, { output, outputTensor }},
//					{
//						applyW1, applyW2, applyW3,
//						applyB1, applyB2, applyB3,
//						layer3
//					},
//					nullptr
//				));
				
				TF_CHECK_OK(session.Run(
					{{ input, inputTensor }, { nextQ, nextQTensor }, { output, outputTensor }},
					{
						applyW1, // applyW2, applyW3,
						applyB1, // applyB2, applyB3,
						lastLayer, loss
					},
					&tfOutputsPrint
				));
				
				std::cout << "[LOSS]: " << tfOutputsPrint[3].DebugString() << std::endl;
			}
		}
		
		noise = ::Utils::Math::Lerp(noise, qoptions.minRandActionChance, qoptions.randActionDecayFactor);

		printf(
			"End of episode: %d with a reward of %.2f. Noise: %.2f\n",
			episode,
			episodeReward,
			noise);
	}
	return 0;
}

