#include "EventHandler.h"
#include "GameWindow.h"
#include "Controller.h"

#include <SDL.h>

namespace GameView
{

EventHandler::EventHandler()
{}



EventHandler::~EventHandler()
{}

void EventHandler::processInput()
{
   SDL_Event currentEvent;

   while (SDL_PollEvent(&currentEvent)) {
      if (currentEvent.type == SDL_KEYDOWN) {
         m_controller->processInput(currentEvent);
      }

      if (currentEvent.type == SDL_QUIT) {
         m_gameWindow->exitGame();
      }

   }

}

void EventHandler::addGameWindow(GameWindow*  gameWindow)
{
   m_gameWindow = gameWindow;
}

void EventHandler::addGameController(Controller*  controller)
{
   m_controller = controller;
}

}