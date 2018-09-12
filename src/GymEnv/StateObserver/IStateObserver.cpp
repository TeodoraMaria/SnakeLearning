#include "GymEnv/StateObserver/IStateObserver.hpp"

using namespace GymEnv::StateObserver;
using namespace GameLogic::CellInterpreter;

IStateObserver::IStateObserver(ICellInterpreterPtr cellInterpreter) :
	m_cellInterpreter(cellInterpreter)
{
}

const ICellInterpreter* IStateObserver::GetCellInterpreter() const
{
	return m_cellInterpreter.get();
}
