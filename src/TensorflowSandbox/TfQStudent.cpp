#include "TfQStudent.hpp"
#include "GameLogic/IPlayer.h"

#include "tensorflow/cc/client/client_session.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/cc/framework/gradients.h"

using namespace AI::QLearning;
using namespace tensorflow;
using namespace tensorflow::ops;

TfQStudent::TfQStudent(
	std::shared_ptr<GymEnv::StateObserver::IStateObserver> observer,
	double actionQulityCompareEps,
	double learningRate
) :
	m_observer(observer),
	m_actionQulityCompareEps(actionQulityCompareEps),
	m_learningRate(learningRate),
	m_tfScope(tensorflow::Scope::NewRootScope())
{
	auto& scope = m_tfScope;

	auto input = Placeholder(scope, tensorflow::DataType::DT_FLOAT);
	auto output = Placeholder(scope, tensorflow::DataType::DT_FLOAT);
	
	const long long numInputs = m_observer->NbOfObservations();
	const long long numOutputs = IPlayer::possibleMoves.size();
	
	const auto layersSizes = { numInputs * 4, (numInputs + numOutputs) * 2 };
	
	// Weights.
	const auto shape1 = { numInputs, layersSizes.begin()[0] };
	auto w1 = Variable(scope, shape1, DataType::DT_FLOAT);
	auto assignW1 = Assign(scope, w1, RandomNormal(scope, Input(shape1), DataType::DT_FLOAT));
	
	const auto shape2 = { layersSizes.begin()[0], layersSizes.begin()[1] };
	auto w2 = Variable(scope, shape2, DataType::DT_FLOAT);
	auto assignW2 = Assign(scope, w2, RandomNormal(scope, Input(shape2), DataType::DT_FLOAT));
	
	const auto shape3 = { layersSizes.begin()[1], numOutputs };
	auto w3 = Variable(scope, shape3, DataType::DT_FLOAT);
	auto assignW3 = Assign(scope, w3, RandomNormal(scope, Input(shape3), DataType::DT_FLOAT));
	
	// Bias.
	auto b1 = Variable(scope, { 1, shape1.begin()[1] }, DataType::DT_FLOAT);
	auto assignB1 = Assign(scope, b1, RandomNormal(scope, { 1, shape1.begin()[1] }, DataType::DT_FLOAT));
	
	auto b2 = Variable(scope, { 1, shape2.begin()[1] }, DataType::DT_FLOAT);
	auto assignB2 = Assign(scope, b2, RandomNormal(scope, { 1, shape2.begin()[1] }, DataType::DT_FLOAT));
	
	auto b3 = Variable(scope, { 1, shape3.begin()[1] }, DataType::DT_FLOAT);
	auto assignB3 = Assign(scope, b3, RandomNormal(scope, { 1, shape3.begin()[1] }, DataType::DT_FLOAT));
	
	// Layers.
	auto layer1 = Relu(scope, Add(scope, MatMul(scope, input, w1), b1));
	auto layer2 = Relu(scope, Add(scope, MatMul(scope, layer1, w2), b2));
	auto layer3 = Relu(scope, Add(scope, MatMul(scope, layer2, w3), b3));
	
	auto reqularization = AddN(
		scope,
		std::initializer_list<Input>{ L2Loss(scope, w1), L2Loss(scope, w2), L2Loss(scope, w2) }
	);
	auto loss = Add(
		scope,
		ReduceMean(scope, Square(scope, Sub(scope, layer3, output)), { 0, 1 }),
		Mul(scope, Cast(scope, m_learningRate, DataType::DT_FLOAT), reqularization)
	);
	
	/*
	** Backpropagation.
	*/
	
	std::vector<Output> gradOutputs;
	TF_CHECK_OK(AddSymbolicGradients(scope, { loss }, { w1, w2, w3, b1, b2, b3 }, &gradOutputs));
	
	auto applyW1 = ApplyGradientDescent(scope, w1, Cast(scope, m_learningRate, DataType::DT_FLOAT), { gradOutputs[0] });
	auto applyW2 = ApplyGradientDescent(scope, w2, Cast(scope, m_learningRate, DataType::DT_FLOAT), { gradOutputs[1] });
	auto applyW3 = ApplyGradientDescent(scope, w3, Cast(scope, m_learningRate, DataType::DT_FLOAT), { gradOutputs[2] });

	auto applyB1 = ApplyGradientDescent(scope, b1, Cast(scope, m_learningRate, DataType::DT_FLOAT), { gradOutputs[3] });
	auto applyB2 = ApplyGradientDescent(scope, b2, Cast(scope, m_learningRate, DataType::DT_FLOAT), { gradOutputs[4] });
	auto applyB3 = ApplyGradientDescent(scope, b3, Cast(scope, m_learningRate, DataType::DT_FLOAT), { gradOutputs[5] });
	
	ClientSession session(scope);
	auto outputs = std::vector<Tensor>();
	
	// Init the weights and biases by running the assigns nodes once.
	TF_CHECK_OK(session.Run(
		{ assignW1, assignW2, assignW3, assignB1, assignB2, assignB3 },
		nullptr
	));
	
	
}
