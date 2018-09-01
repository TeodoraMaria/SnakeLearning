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

      void draw() const;

      void drawUpperLeftCorner() const;
      void drawUpperRightCorner() const;
      void drawLowerRightCorner() const;
      void drawlowerLeftCorner() const;

      void drawSnakeHead() const;
      void drawSnakeBody() const;

      void drawFood() const;
      
      void setValue(int value);

      void printConsole() const;

   private:
      void setGlColor() const;

      double m_x = 0;
      double m_y = 0;
      double  m_width = 0;
      double m_height = 0;

      int m_value = 0;
   };
}
