#pragma once

namespace GameView
{
   namespace Utils
   {

      enum class InputDirection
      {
         UP,
         DOWN,
         LEFT,
         RIGHT
      };



         enum class Color
         {
            RED,
            BLUE,
            GREEN,
      };

      /*
      static void convertPixelToGlCoord(double &x, double& y)
      {
         float xx = x / (windowWidth / 2.0f);
         float yy = y / (windowHeight / 2.0f);
         xx -= 1.0f;
         yy -= 1.0f;
      }
      */

   }
}