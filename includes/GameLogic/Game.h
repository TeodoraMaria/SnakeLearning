#include <vector>
#include <list>
#include "IPlayer.h"

class Game
{
private:
	std::vector<std::vector<int>> m_gameBoard;
	std::vector<IPlayer> m_players;
	std::vector<std::list<std::tuple<int, int, int>>> m_snakes;

	bool m_isLimitless;
	size_t m_numberOfFoods;

	void AddLimitsToBoard();
	void InitSnakes();
	void InitFood();
	void AddSnakeToGame(size_t snakeNumber);
	bool CheckCoord(std::tuple<int, int> coord) const;
	int GenerateCoordinate() const;
public:
	Game(bool limitless = false, size_t boardDimension = 10, size_t numberOfPlayers = 1, size_t foodNo = 1);
	~Game();
	void InitGame();
	std::vector<std::vector<int>> GetGameBoard() const;
	std::vector<std::list<std::tuple<int, int, int>>> GetSnakes() const;
};