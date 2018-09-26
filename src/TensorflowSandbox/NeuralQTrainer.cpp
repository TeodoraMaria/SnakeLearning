#include "NeuralQTrainer.hpp"
#include "NeuralQAgent.hpp"
#include "TrainMemory.h"
#include "TrainSession.h"

#include "GameLogic/CellInterpreter/Basic3CellInterpreter.hpp"
#include "GymEnv/StateObserver/GridObserver.hpp"
#include "GameLogic/IPlayer.h"

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

// Json
#include "ConfigLoading/QTabStudentJson.h"
#include "ConfigLoading/IObserverJson.h"
#include "ConfigLoading/ICellInterpreterJson.h"
#include "AI/QLearning/TabularQJsonUtils.h"
#include <json.hpp>

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

using json = nlohmann::json;

using namespace tensorflow;
using namespace tensorflow::ops;

using namespace AI::QLearning;
using namespace GameLogic;
using namespace GameLogic::CellInterpreter;
using namespace GymEnv::StateObserver;
using namespace GymEnv::Utils;

static const int NoneFlag = -42;
//static auto g_debugging = true;
static auto g_debugging = false;


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
	
	// Percentage of mean.
	qoptions.maxNoise = 1.5;
	qoptions.minNoise = 0.02;
	qoptions.noiseDecayFactor = [](int numEpisodes) { return 1.0 / (numEpisodes * 0.1); };
	
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

IPlayerPtr NeuralQTrainer::Train(TrainCallbacks callbacks)
{
	auto scopePtr = std::make_shared<Scope>(Scope::NewRootScope());
	auto sessionPtr = std::make_shared<ClientSession>(*scopePtr);

	auto agents = std::vector<std::shared_ptr<NeuralQAgent>>();
	auto agentWeightsFile = std::string("./aux_files/qneural/network.json");
	
	for (auto i = 0; i < 1; i++)
	{
		auto cellInterpreter = std::make_shared<CellInterpreter::Basic3CellInterpreter>();
		auto observer = std::make_shared<GridObserver>(cellInterpreter, 5, 5);
		auto agent = std::make_shared<NeuralQAgent>(scopePtr, sessionPtr, observer);
	
		try
		{
			agent->LoadWeights(agentWeightsFile);
		}
		catch (...)
		{
			std::cout << "[Warning] Failed to load network" << std::endl;
		}
		
		agents.push_back(agent);
	}
	
	auto gmOptions = GetGameOptions();
	auto qoptions = GetQOptions();
	qoptions.numEpisodes = callbacks.numEpisodes;

	auto players = std::vector<std::shared_ptr<IPlayer>>();
	for (auto agent : agents)
		players.push_back(std::static_pointer_cast<IPlayer>(agent));

	auto game = Game(gmOptions, players);
	
	auto merseneTwister = std::mt19937(std::random_device()());
	auto requiredOutput = std::vector<float>(IPlayer::possibleMoves.size());
	auto startedToRender = false;

	auto noise = qoptions.maxNoise;
	if (g_debugging)
		std::cout << std::endl << std::endl;

	for (auto episode = 0u; episode < qoptions.numEpisodes; episode++)
	{
		if ((episode >= qoptions.numEpisodes - qoptions.lastNGamesToRender) && !startedToRender)
		{
			startedToRender = true;
			std::system("play -q -n synth 1 sin 880");
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	
		game.InitGame();
		const auto maxNbOfSteps = qoptions.maxNumSteps(episode);
		const auto maxStepsWithoutFood = qoptions.maxStepsWithoutFood(episode);

		auto trainSessions = std::unordered_map<int, TrainSession>();
		for (auto& agent : agents)
		{
			trainSessions[agent->GetSnakeNumber()] = TrainSession();
		}

		for (auto step = 0u; step < maxNbOfSteps; step++)
		{
			if (game.EveryoneIsDead())
				break;
			
			for (auto& agent : agents)
			{
				const auto snakeId = agent->GetSnakeNumber();
				const auto gmState = game.GetGameState();
				auto& trainSession = trainSessions[snakeId];
				
				if (!gmState.GetSnake(snakeId).IsAlive())
					continue;
				
				if (trainSession.stepsWithoutFood >= maxStepsWithoutFood)
				{
					game.ForcefullyKillPlayer(snakeId);
					continue;
				}
				
				int actionIndx = agent->GetNextActionIndex(
					gmState,
					noise,
					merseneTwister,
					qoptions.actionQualityEps);

				if (episode >= qoptions.numEpisodes - qoptions.lastNGamesToRender)
				{
					
					gmOptions.gameRenderer->Render(game.GetGameState());
					if (qoptions.milsToSleepBetweenFrames != 0)
						std::this_thread::sleep_for(std::chrono::milliseconds(qoptions.milsToSleepBetweenFrames));
				}
				
				const auto action = IPlayer::possibleMoves[actionIndx];
				const auto rawReward = game.MoveSnake(snakeId, action);

				auto reward = qoptions.ComputeReward(episode, rawReward);
				if (rawReward > 0)
					trainSession.stepsWithoutFood = 0;
				else if (rawReward == 0)
					trainSession.stepsWithoutFood++;

				trainSession.episodeReward += reward;
				const auto isDone = (rawReward < 0);
				
				if (isDone)
					agent->nextObservationContainer[0] = NoneFlag;
				else
				{
					const auto gmStatePrim = game.GetGameState();
					agent->observer->Observe(agent->nextObservationContainer, gmStatePrim, snakeId);
				}
				trainSession.trainMemory.RegisterMemory(
					agent->observationContainer,
					actionIndx,
					reward,
					agent->nextObservationContainer);
				game.RestockFood();
			}
		}
		
		for (auto& agent : agents)
		{
			auto& trainSession = trainSessions[agent->GetSnakeNumber()];
			auto& trainMemory = trainSession.trainMemory;
		
			const auto discountedRewards = trainMemory.GetDiscountedRewards(qoptions.qDiscountFactor);
			for (int batch_i = trainMemory.nbOfMemories - 1; batch_i >= 0; batch_i--)
			{
				const auto batchSize = 1;

				std::vector<std::vector<double>> observationsBatch = { trainMemory.states[batch_i] };
				std::vector<std::vector<double>> nextObservationsBatch = { trainMemory.nextStates[batch_i] };
				
				auto inputBatch = MakeTensor(observationsBatch);
				auto q_s_a = agent->PredictBatch(inputBatch);
				auto q_s_a_d = agent->PredictBatch(MakeTensor(nextObservationsBatch));

				auto trainOutputBatch = Tensor(
					DataTypeToEnum<float>::v(),
					TensorShape{ (int)batchSize, (int)agent->NumOutputs() });

				std::copy_n(
					q_s_a.matrix<float>().data(),
					batchSize * agent->NumOutputs(),
					trainOutputBatch.matrix<float>().data()
				);

				const auto actionIndx = trainMemory.actions[batch_i];
				float finalActionReward = discountedRewards[batch_i];
				
				if (!StateIsNone(trainMemory.nextStates[batch_i].data()))
				{
					const float* q_s_a_d_raw = q_s_a_d.matrix<float>().data();
					const auto bestNextQ = *std::max_element(
						q_s_a_d_raw,
						q_s_a_d_raw + agent->NumOutputs());

					finalActionReward += qoptions.qDiscountFactor * bestNextQ;
				}
				trainOutputBatch.matrix<float>()(actionIndx) = finalActionReward;
				
				agent->Train(inputBatch, trainOutputBatch);
			}
		}

		printf("End of episode: %5d. Die reward: %3.2f\n", episode, qoptions.dieReward(episode));
		for (const auto& agent : agents)
		{
			const auto& trainSession = trainSessions[agent->GetSnakeNumber()];
			printf("- %d) Reward =\033[32m %5.2f\033[0m. Noise = %2.2f %s\n",
				agent->GetSnakeNumber(),
				trainSession.episodeReward,
				noise,
				(trainSession.stepsWithoutFood >= maxStepsWithoutFood) ? "\033[31m - Starved - \033[0m" : "");
		}
		noise = ::Utils::Math::Lerp(
			noise,
			qoptions.minNoise,
			qoptions.noiseDecayFactor(callbacks.numEpisodes));
		
		/*
		** Callbacks.
		*/
		
		if (callbacks.emitStepEpisode)
			callbacks.emitStepEpisode(static_cast<size_t>(episode));
		
		// Emit graph.
		if (callbacks.emitGraphValues)
		{
			auto graphRewards = std::vector<double>();
			auto rewardSum = 0.0;
			for (const auto& agent : agents)
			{
				const auto reward = trainSessions[agent->GetSnakeNumber()].episodeReward;
				
				rewardSum += reward;
				graphRewards.push_back(reward);
			}
			graphRewards.push_back(rewardSum / agents.size());
			
			// I want the mean to be the first one.
			std::reverse(graphRewards.begin(), graphRewards.end());
			callbacks.emitGraphValues(graphRewards);
		}
		
		if (callbacks.emitDisplayGame)
			callbacks.emitDisplayGame(agents[0], 1000);
	}
	
	try
	{
		
		agents[0]->SaveWeights(agentWeightsFile);
	}
	catch (...)
	{
		// Ignore.
	}
	
	auto jsonAgentStream = std::ofstream("./aux_files/qneural/NeuralQAgent.json");
	if (!jsonAgentStream.is_open())
		throw std::runtime_error("Failed to open file");
	
	json j(agents[0].get());
	jsonAgentStream << std::setw(2) << j << std::endl;
	jsonAgentStream.close();
	
	return j.get<std::shared_ptr<NeuralQAgent>>();
}
