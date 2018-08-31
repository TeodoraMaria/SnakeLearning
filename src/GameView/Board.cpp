#include "Board.h"
#include "utils.h"
#include <iostream>

namespace GameView
{
   Board::Board()
   {}

   Board::Board(const size_t & rows, const size_t & cols) :m_rows(rows), m_cols(cols)
   {
      /*
      size_t widthOffset = Utils::windowWidth / rows;
      size_t heightOffset = Utils::windowHeight / cols;

      m_gameCells.resize(rows);


      for (size_t i = 0; i < rows; i++) {

         m_gameCells[i].resize(cols);

         for (size_t j = 0; j < cols; j++) {

            m_gameCells[i][j] = Cell(i*widthOffset, j*heightOffset, widthOffset, heightOffset);
         }
      }
      */
   }

   Board::Board(std::vector<std::vector<int>> board, size_t windowWidth, size_t windowHeigth)
   {
      size_t rows = board.size();
      size_t cols = board[0].size();
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


   Board::~Board()
   {}

}
