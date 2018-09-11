#include "SingleBot.hpp"
#include <random>
#include <cstddef>

using namespace AI::HardCoded;

/*
** Utils
*/

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

/*
** The actual Bot implementation.
*/

SnakeMove SingleBot::GetNextAction(const GameState& gameState)
{
	const Snake& snake = gameState.GetSnake(GetSnakeNumber());

	const GameBoard& board = gameState.GetGameBoard();
	const Coordinate& head = snake.GetSnakeHead();

	const Coordinate snakeOrientation = snake.GetOrientation();
	const Coordinate left = head + snakeOrientation.Rotate90Left();
	const Coordinate right = head + snakeOrientation.Rotate90Right();
	const Coordinate forward = head + snakeOrientation;

	const auto isValidPos = [&](const Coordinate& coord)
	{
		return
			(board[coord] == BoardPart::EMPTY) ||
			(board[coord] == BoardPart::FOOD);
	};
	
	auto possibleMoves = std::vector<SnakeMove>();
	auto possibleCoordMoves = std::vector<Coordinate>();
	const auto addMoveIfValid = [&](const Coordinate& coord, const SnakeMove mv)
	{
		if (isValidPos(coord))
		{
			possibleMoves.push_back(mv);
			possibleCoordMoves.push_back(coord);
		}
	};
	
	addMoveIfValid(left, SnakeMove::LEFT);
	addMoveIfValid(right, SnakeMove::RIGHT);
	addMoveIfValid(forward, SnakeMove::FORWARD);
	
	for (auto i = 0u; i < possibleCoordMoves.size(); i++)
	{
		if (board[possibleCoordMoves[i]] == BoardPart::FOOD)
			return possibleMoves[i];
	}
	
	if (possibleMoves.size() == 0)
		return SnakeMove::FORWARD;
	
	return *select_randomly(possibleMoves.begin(), possibleMoves.end());
}
