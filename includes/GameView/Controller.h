#pragma once
#include <vector>
#include <memory>

#include "GameLogic/Game.h"
#include "GameLogic/HumanPlayer.h"

#include "HumanPlayer2.h"
#include "GameLogic/IPlayer.h"
#include "GraphPlotter.h"

union SDL_Event;

namespace GameView
{

class Board;
class EventHandler;

class Controller
{
public:
   Controller();
   ~Controller();

   void processInput(const SDL_Event& keyPressed);
   void updateBoard();
   void addBoard(Board* board);
   void addGraphPlotter(GraphPlotter* graphPlotter);

   GameState getGameState()const;

   bool sendActions();
   void resetPlayersInput();

   size_t getAliveSnakes() const;
   size_t getCols() const;
   size_t getLines()const;

private:
   void processInputPlayer1(const SDL_Event& keyPressed);
   void processInputPlayer2(const SDL_Event& keyPressed);
   std::vector<IPlayerPtr> m_players;

   Game* m_game;
   Board* m_board;

   GraphPlotter* m_graphPlotter=nullptr;

   size_t m_lastTime = 0;
   size_t m_currentTime;
};

}
