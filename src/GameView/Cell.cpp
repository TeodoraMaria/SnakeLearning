#include "Cell.h"
#include <GL\glew.h>
#include "utils.h"

#include <iostream>


namespace GameView
{

   Cell::Cell()
   {

   }

   Cell::Cell(double x, double y, double width, double height, int value) 
      :m_x(x), m_y(y), m_width(width), m_height(height), m_value(value)
   {

   }

   Cell::~Cell()
   {}

   void Cell::draw() const
   {
      setGlColor();

      glBegin(GL_TRIANGLES);

      glVertex2d(m_x, m_y);
      glVertex2d(m_x + m_width, m_y);
      glVertex2d(m_x, m_y + m_height);

      glVertex2d(m_x + m_width, m_y);
      glVertex2d(m_x, m_y + m_height);
      glVertex2d(m_x + m_width, m_y + m_height);
      glEnd();
   }

   void Cell::drawUpperLeftCorner() const
   {}

   void Cell::drawUpperRightCorner() const
   {}

   void Cell::drawLowerRightCorner() const
   {}

   void Cell::drawlowerLeftCorner() const
   {}

   void Cell::drawSnakeHead() const
   {}

   void Cell::drawSnakeBody() const
   {}

   void Cell::drawFood() const
   {}

   void Cell::setValue(int value)
   {
      m_value = value;
   }

   void Cell::printConsole() const
   {
      std::cout << m_x << " " << m_y << " " << m_height << " " << m_width << std::endl;
   }

   void Cell::setGlColor() const
   {
      switch (m_value) {
         case -1: {
            glColor3f(1.0, 0.0, 0.0);
            break;
         }
         case 1: {
            glColor3f(0.0, 0.0, 1.0);
            break;
         }
         case 0: {
            glColor3f(0.0, 1.0, 0.0);
            break;
         }
         default: {
            glColor3f(0.0, 0.0, 0.0);
            break;
         }
      }
   }
}


