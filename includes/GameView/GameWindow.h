#pragma once

#include "Board.h"
#include "Controller.h"
#include <sdl\SDL.h>

namespace GameView
{

   enum class EGameState
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
      void initSystems();
      void processInput();
      void gameLoop();
      void drawGame();

      void exitGame();

   private:

      SDL_Window* m_window;

      size_t m_screenWidth;
      size_t m_screenHeight;

      EGameState m_gameSate = EGameState::PLAY;

      Board* m_board;

      Controller m_controller;

   };
}

