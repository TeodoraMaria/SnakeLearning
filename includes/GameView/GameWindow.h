#pragma once

#include "Board.h"
#include "Controller.h"
#include "OpenGLRenderer.h"
#include "TermRenderer.hpp"
#include "GraphPlotter.h"

#include <memory>


namespace GameView
{
   class EventHandler;

   enum class GameState
   {
      PLAY,
      EXIT
   };

   class GameWindow
   {
   public:
      GameWindow(size_t screenWidth, size_t screenHeight);
      ~GameWindow();

      void run();
      
      void processInput();
      void gameLoop();
      void drawGame();

      void exitGame();

   private:

      size_t m_screenWidth;
      size_t m_screenHeight;

      GameState m_gameSate = GameState::PLAY;
      
      OpenGLRenderer m_renderer;

      std::shared_ptr<GraphPlotter> m_graphPlotter;
      

      std::unique_ptr<Board> m_board;
      std::unique_ptr<Controller> m_controller;
      std::unique_ptr<EventHandler> m_eventHandler;
   };
}

