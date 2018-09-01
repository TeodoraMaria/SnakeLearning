#include "Cell.h"
#include <GL\glew.h>
#include "utils.h"

#include <iostream>


namespace GameView
{

   Cell::Cell()
   {

   }

   Cell::Cell(double x, double y, double width, double height, int value) :m_x(x), m_y(y), m_width(width), m_height(height), m_value(value)
   {

   }


   Cell::~Cell()
   {}

   void Cell::draw()
   {
      setGlColor2();

      glBegin(GL_TRIANGLES);

      glVertex2d(m_x, m_y);
      glVertex2d(m_x + m_width, m_y);
      glVertex2d(m_x, m_y + m_height);

      glVertex2d(m_x + m_width, m_y);
      glVertex2d(m_x, m_y + m_height);
      glVertex2d(m_x + m_width, m_y + m_height);
      glEnd();
   }

   void Cell::drawUpperLeftCorner()
   {}

   void Cell::drawUpperRightCorner()
   {}

   void Cell::drawLowerRightCorner()
   {}

   void Cell::drawlowerLeftCorner()
   {}

   void Cell::drawSnakeHead()
   {}

   void Cell::drawSnakeBody()
   {}

   void Cell::drawFood()
   {}

   void Cell::setValue(int value)
   {
      m_value = value;
   }

   void Cell::printConsole()
   {
      std::cout << m_x << " " << m_y << " " << m_height << " " << m_width << std::endl;
   }

   void Cell::setGLColor(Utils::Color color)
   {
      switch (color) {
         case Utils::Color::RED: {
            glColor3f(1.0, 0.0, 0.0);
            break;
         }
         case Utils::Color::BLUE: {
            glColor3f(0.0, 0.0, 1.0);
            break;
         }
         case Utils::Color::GREEN: {
            glColor3f(0.0, 1.0, 0.0);
            break;
         }
         default: {
            glColor3f(0.0, 0.0, 0.0);
            break;
         }
      }
   }

   void Cell::setGlColor2()
   {
      switch (m_value) {
         case -1: {
            glColor3f(1.0, 0.0, 0.0);
            break;
         }
         case 11: {
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


