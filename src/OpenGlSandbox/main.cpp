#include <time.h>
#include <iostream>

#include "GameView/GameWindow.h"

int main(int argc, char** argv)
{
   srand(time(NULL));
   //srand(1234);

   SDL_Init(SDL_INIT_EVERYTHING);

   GameView::GameWindow gamewindow(500, 500);

   gamewindow.run();

     
   std::cout<<"1"<<std::endl;

   return 0;
}