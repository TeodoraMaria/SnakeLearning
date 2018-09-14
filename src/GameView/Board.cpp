#include "Board.h"
#include "utils.h"
#include "GameLogic/GameBoard.h"
#include <iostream>

namespace GameView
{
   Board::Board()
   {}

   Board::Board(size_t windowWidth, size_t windowHeigtht):m_windowWidth(windowWidth),m_windowHeight(windowHeigtht)
   {
      
   }
   void Board::draw()
   {
      for (const auto& cell : m_gameCells) {
            cell.draw();
      }
   }

   void Board::setCellValueAt(int i, int j, int value)
   {
      m_gameCells[i*m_cols+j].setValue(value);
   }

   size_t Board::getWidth() const
   {
      return m_windowWidth;
   }

   size_t Board::getHeight() const
   {
      return m_windowHeight;
   }

   void Board::setUpBoard(int rows, int cols)
   {
       m_rows = rows;
       m_cols = cols;
      size_t widthOffset = m_windowWidth / rows;
      size_t heightOffset = m_windowHeight / cols;

      m_gameCells.resize(cols*rows);

      for (size_t i = 0; i < cols; i++) {
         //m_gameCells[i].resize(rows);

         for (size_t j = 0; j < rows; j++) {
            m_gameCells[i*cols+j] = Cell(i*widthOffset, j*heightOffset, widthOffset, heightOffset, BoardPart::EMPTY);
         }
      }
   }
   Board::~Board()
   {}

}
