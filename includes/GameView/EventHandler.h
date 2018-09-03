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
   std::shared_ptr<GameWindow> m_gameWindow;
   std::shared_ptr<Controller> m_controller;
};

}


