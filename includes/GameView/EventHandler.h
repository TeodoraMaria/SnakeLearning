#pragma once
#include "Controller.h"
#include "GameWindow.h"
#include <memory>

namespace GameView
{


class EventHandler
{
public:
   EventHandler();
   ~EventHandler();

   void processInput();

   void addGameWindow(GameWindow& gameWindow);
   void addGameController(Controller& controller);

private:
   GameWindow* m_gameWindow;
   Controller* m_controller;
};

}


