#include "Controller.h"
#include <SDL.h>



Controller::Controller()
{}


Controller::~Controller()
{}

void Controller::processInput(const SDL_Event& currentEvent)
{
      if (currentEvent.type == SDL_KEYDOWN) {
         switch (currentEvent.key.keysym.sym) {
            case SDLK_w: {
              // translate(0, 0.2);
               break;
            }
            case SDLK_s: {
              // translate(0, -0.2);
               break;
            }
            case SDLK_d: {
              // translate(0.2, 0);
               break;
            }
            case SDLK_a: {
               //translate(-0.2, 0);
               break;
            }
         }
      }
 
}

void Controller::setPlayerDirection()
{
   currentTime = SDL_GetTicks();
   if (currentTime > lastTime + 1000) {
      lastTime = currentTime;

      

   }

}
