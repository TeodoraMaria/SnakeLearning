#include "Board.h"
#include "utils.h"
#include <iostream>

namespace GameView
{
   Board::Board()
   {}

   Board::Board(std::vector<std::vector<int>> board, size_t windowWidth, size_t windowHeigth)
   {
      //size_t rows = board.size();
      //size_t cols = board[0].size();
      size_t rows = 20;
      size_t cols = 10;
      size_t widthOffset = windowWidth / rows;
      size_t heightOffset = windowHeigth / cols;

      m_gameCells.resize(rows);


      for (size_t i = 0; i < rows; i++) {

         m_gameCells[i].resize(cols);

         for (size_t j = 0; j < cols; j++) {

            m_gameCells[i][j] = Cell(i*widthOffset, j*heightOffset, widthOffset, heightOffset, board[i][j]);
         }
      }

   }

   Board::Board(int rows, int cols, size_t windowWidth, size_t windowHeigth)
   {
      m_rows = rows;
      m_cols = cols;
      size_t widthOffset = windowWidth / rows;
      size_t heightOffset = windowHeigth / cols;

      m_gameCells.resize(cols);


      for (size_t i = 0; i < cols; i++) {

         m_gameCells[i].resize(rows);

         for (size_t j = 0; j < rows; j++) {

            m_gameCells[i][j] = Cell(i*widthOffset, j*heightOffset, widthOffset, heightOffset, 0);
         }
      }
   
   }

   Board::Board(size_t windowWidth, size_t windowHeigtht):m_windowWidth(windowWidth),m_windowHeight(windowHeigtht)
   {
      
   }

   void Board::consolePrint()
   {
      for (auto cellVec : m_gameCells) {
         for (auto cell : cellVec) {
            cell.printConsole();
         }
      }
   }

   void Board::draw()
   {
      for (auto cellVec : m_gameCells) {
         for (auto cell : cellVec) {
            cell.draw();
         }
      }
   }

   void Board::setCellAt(int i, int j, int value)
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

            m_gameCells[i][j] = Cell(i*widthOffset, j*heightOffset, widthOffset, heightOffset, 0);
         }
      }
   }


   Board::~Board()
   {}

}
