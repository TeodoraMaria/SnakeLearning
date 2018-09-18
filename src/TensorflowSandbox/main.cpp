#include "GameLogic/CellInterpreter/Basic3CellInterpreter.hpp"
#include "GymEnv/StateObserver/GridObserver.hpp"

#include <tensorflow/cc/ops/standard_ops.h>
#include <tensorflow/cc/client/client_session.h>
#include <tensorflow/cc/framework/gradients.h>
#include <tensorflow/core/framework/tensor.h>

#include <iostream>

using namespace tensorflow;
using namespace tensorflow::ops;

using namespace GameLogic;
using namespace GymEnv::StateObserver;

int main()
{
	auto cellInterpreter = std::make_shared<CellInterpreter::Basic3CellInterpreter>();
	auto observer = std::make_shared<GridObserver>(cellInterpreter, 5, 5);
	
	
	return 0;
}

