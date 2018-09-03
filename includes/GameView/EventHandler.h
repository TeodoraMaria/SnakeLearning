#pragma once
#include <memory>

namespace GameView
{
   class GameWindow;
   class Controller;

class EventHandler
{
public:
   EventHandler();
   ~EventHandler();

   void processInput();

   void addGameWindow(GameWindow* gameWindow);
   void addGameController(Controller* controller);

   void checkIfGameOver();

private:
   GameWindow* m_gameWindow;
   Controller* m_controller;
};

}


