#pragma once
#include <vector>
#include <SDL.h>


#include "GameLogic/Game.h"
#include "GameLogic/HumanPlayer.h"

namespace GameView
{

class Board;
class EventHandler;

enum class InputDirection
{
   UP,
   DOWN,
   LEFT,
   RIGHT
};


class Controller
{
public:
   Controller();
   Controller(EventHandler* eventHandler);
   ~Controller();

   void processInput(const SDL_Event& keyPressed);

   void updateBoard(Board* board);
   void sendActions();

   void setPlayerDirection();
   void setBoard(Board* board);

private:

   size_t lastTime = 0;
   size_t currentTime;


   std::vector<InputDirection> m_playersDirection;

   Game* m_game;
   EventHandler* m_eventHandler;

   

};

}
