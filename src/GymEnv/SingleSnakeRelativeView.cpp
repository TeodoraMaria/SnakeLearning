#include "SingleSnakeRelativeView.hpp"
#include "SnakeStudent.hpp"
#include <cmath>
#include <assert.h>

using namespace GymEnv;

SingleSnakeRelativeView::SingleSnakeRelativeView(
	GameView::IGameRenderer* gameRenderer,
	const GameOptions& gmOptions) :
	SingleSnakeEnvBase(gameRenderer, gmOptions)
{
	auto players = std::vector<IPlayerPtr>(
	{
		IPlayerPtr(m_student)
	});
	
	auto gameOptions = GameOptions();
	{
		gameOptions.gameBoardType = GameBoardType::BOX;
		gameOptions.boardWidth = 10;
		gameOptions.boardLength = 10;
		gameOptions.numberOfPlayers = players.size();
		gameOptions.numFoods = 1;
	}
	
	m_game = std::make_unique<Game>(Game(gameOptions, players));
}

size_t SingleSnakeRelativeView::GetNumbOfObservations() const
{
	// 0 - Nothing, 1 - Wall, 2 - Food.
	static const size_t cellStatesCount = 3;
	static const size_t viewGridSize = 3;
	
	return std::pow(viewGridSize, cellStatesCount);
}

int SingleSnakeRelativeView::GetState() const
{
	return GymEnv::Utils::StateExtractor::GetRelativeViewStateBase3(
		m_game->GetGameState(),
		m_student->GetSnakeNumber());
}
