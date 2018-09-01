#pragma once
#include <time.h>

#include "GameWindow.h"
   int main(int argc, char** argv)
   {
      srand(time(NULL));

      SDL_Init(SDL_INIT_EVERYTHING);

      GameView::GameWindow gamewindow(500, 500);

      gamewindow.run();

      return 0;
   }
