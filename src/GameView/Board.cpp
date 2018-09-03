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
      for (const auto& cellVec : m_gameCells) {
         for (const auto& cell : cellVec) {
            cell.draw();
         }
      }
   }

   void Board::setCellValueAt(int i, int j, int value)
   {
      m_gameCells[i][j].setValue(value);
   }

   void Board::setUpBoard(int rows, int cols)
   {
      size_t widthOffset = m_windowWidth / rows;
      size_t heightOffset = m_windowHeight / cols;

      m_gameCells.resize(cols);

      for (size_t i = 0; i < cols; i++) {
         m_gameCells[i].resize(rows);

         for (size_t j = 0; j < rows; j++) {
            m_gameCells[i][j] = Cell(i*widthOffset, j*heightOffset, widthOffset, heightOffset, BoardPart::EMPTY);
         }
      }
   }
   Board::~Board()
   {}

}
