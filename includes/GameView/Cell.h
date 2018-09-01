#pragma once

#include "utils.h"

namespace GameView
{


   class Cell
   {
   public:
      Cell();
      Cell(double x, double y, double width, double height, int value);
      ~Cell();

      void draw();

      void drawUpperLeftCorner();
      void drawUpperRightCorner();
      void drawLowerRightCorner();
      void drawlowerLeftCorner();

      void drawSnakeHead();
      void drawSnakeBody();

      void drawFood();
      
      void setValue(int value);

      void printConsole();

   private:

      double m_x = 0;
      double m_y = 0;
      double  m_width = 0;
      double m_height = 0;

      int m_value = 0;

      void setGLColor(Utils::Color color);
      void setGlColor2();

   };
}
