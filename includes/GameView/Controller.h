#pragma once
#include <vector>
#include <SDL.h>


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
   void sendActions();

   void setPlayerDirection();
   void setBoard(Board* board);

private:

   std::vector<Utils::InputDirection> m_playersDirection;
   std::vector<IPlayerPtr> m_players;

   Game* m_game;
   EventHandler* m_eventHandler;

   size_t m_lastTime = 0;
   size_t m_currentTime;
   
};

}
