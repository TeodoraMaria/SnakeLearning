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
   Cell::Cell(double x, double y, double width, double height, int value,Utils::CellType celltype)
      : m_x(x), m_y(y), m_width(width), m_height(height), m_value(value), m_cellType(celltype)
   {

   }

   Cell::~Cell()
   {}

   void Cell::draw() const
   {
      setGlColor();

      glBegin(GL_TRIANGLES);

      switch (m_cellType) {
         case GameView::Utils::CellType::WALL:
            break;
         case GameView::Utils::CellType::FIELD:
            break;
         case GameView::Utils::CellType::FOOD1:
            break;
         case GameView::Utils::CellType::FOOD2:
            break;
         case GameView::Utils::CellType::FOOD3:
            break;
         case GameView::Utils::CellType::SNAKE_HEAD_N:
            break;
         case GameView::Utils::CellType::SNAKE_TAIL_N:
            break;
         case GameView::Utils::CellType::SNAKE_BODY_HORIZONTAL:
            break;
         case GameView::Utils::CellType::SNAKE_BODY_VERTICAL:
            break;
         case GameView::Utils::CellType::SNAKE_BODY_BENT_NE:
            break;
         case GameView::Utils::CellType::SNAKE_BODY_BENT_SE:
            break;
         case GameView::Utils::CellType::SNAKE_BODY_BENT_SW:
            break;
         case GameView::Utils::CellType::SNAKE_BODY_BENT_NW:
            break;
         default:
            drawWall();
            break;
      }



      glEnd();
   }

   void Cell::drawBlock()const
   {
      glVertex2d(m_x, m_y);
      glVertex2d(m_x + m_width, m_y);
      glVertex2d(m_x, m_y + m_height);

      glVertex2d(m_x + m_width, m_y);
      glVertex2d(m_x, m_y + m_height);
      glVertex2d(m_x + m_width, m_y + m_height);
   }

   void Cell::drawWall() const
   {
      double xA = 20;
      double yA = 20;

      double xB = 80;
      double yB = 20;

      double xC = 20;
      double yC = 80;

      double xD = 80;
      double yD = 80;

      xA = Utils::ruleOfThree(xA, m_width, 100);
      xB = Utils::ruleOfThree(xB, m_width, 100);
      xC = Utils::ruleOfThree(xC, m_width, 100);
      xD = Utils::ruleOfThree(xD, m_width, 100);

      yA = Utils::ruleOfThree(yA, m_height, 100);
      yB = Utils::ruleOfThree(yB, m_height, 100);
      yC = Utils::ruleOfThree(yC, m_height, 100);
      yD = Utils::ruleOfThree(yD, m_height, 100);

      glVertex2d(m_x + xA, m_y + yA);
      glVertex2d(m_x + xB, m_y + yB);
      glVertex2d(m_x + xC, m_y + yC);

      glVertex2d(m_x + xB, m_y + yB);
      glVertex2d(m_x + xC, m_y + yC);
      glVertex2d(m_x + xD, m_y + yD);
   
   }

   void Cell::drawField() const
   {}

   void Cell::drawSnakeTail() const
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

   void Cell::setCellType(Utils::CellType cellType)
   {
      m_cellType = cellType;
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
         case 11:{
            glColor3f(1.0, 1.0, 0.0);
            break;
         }
         case 12: {
            glColor3f(0.0, 1.0, 1.0);
            break;
         }
         default: {
            glColor3f(0.0, 0.0, 0.0);
            break;
         }
      }
   }
}


