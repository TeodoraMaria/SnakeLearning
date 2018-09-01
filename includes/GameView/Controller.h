#pragma once
#include <vector>
#include <SDL.h>
#include <memory>


#include "GameLogic/Game.h"
#include "GameLogic/HumanPlayer.h"

#include "HumanPlayer2.h"
#include "GameLogic/IPlayer.h"

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
   void updateBoard(Board* board);
   void setBoard(Board* board);
   void sendActions();

private:
   std::vector<IPlayerPtr> m_players;

   std::shared_ptr<Game> m_game;
   std::shared_ptr<EventHandler> m_eventHandler;

   size_t m_lastTime = 0;
   size_t m_currentTime;
};

}
