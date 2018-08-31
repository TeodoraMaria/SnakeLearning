#pragma once

#include "GameWindow.h"
   int main(int argc, char** argv)
   {
      SDL_Init(SDL_INIT_EVERYTHING);

      // mainGame.run();

      GameView::GameWindow gamewindow(500, 500);

      gamewindow.run();

      return 0;
   }
