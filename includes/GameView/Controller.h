#pragma once
#include <vector>
#include <SDL.h>

class Board;


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
   ~Controller();

   void processInput(const SDL_Event& keyPressed);

   void setPlayerDirection();

private:

   size_t lastTime = 0;
   size_t currentTime;


   std::vector<InputDirection> m_playersDirection;
};

