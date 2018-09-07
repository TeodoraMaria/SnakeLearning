#include "MultiSnakeEnv.hpp"

using namespace GymEnv;

MultiSnakeEnv::MultiSnakeEnv(const MultiSnakeEnvModel& model) :
	SnakeEnvBase(model.gameRenderer),
	m_agents(model.agents)
{
	auto iplayers = std::vector<IPlayerPtr>();
	
	for (auto agent : m_agents)
		iplayers.push_back(std::static_pointer_cast<IPlayer>(agent));
	m_game = std::make_unique<Game>(*model.gmOptions, iplayers);
}

const GameState MultiSnakeEnv::GetState() const
{
	return m_game->GetGameState();
}

void MultiSnakeEnv::Reset()
{
	m_game->InitGame();
}

StepResult MultiSnakeEnv::Step(int snakeId, SnakeMove move)
{
	auto stepResult = StepResult();
	
	stepResult.reward = m_game->MoveSnake(snakeId, move);
	stepResult.isDone = (stepResult.reward < 0);
	
	return stepResult;
}
