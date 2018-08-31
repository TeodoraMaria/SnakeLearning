#pragma once

#include "Board.h"
//#include <sdl\SDL.h>

#define GLEW_STATIC

#include <SDL.h>


namespace GameView
{

   enum class EGameState
   {
      play,
      exit
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

   private:

      SDL_Window* m_window;

      size_t m_screenWidth;
      size_t m_screenHeight;

      EGameState m_gameSate = EGameState::play;

      Board* m_board;

   };
}

