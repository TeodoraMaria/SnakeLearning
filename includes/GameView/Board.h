#pragma once

#include <vector>

#include "Cell.h"
namespace GameView
{
   class Board
   {
   public:
      Board();
      Board(size_t windowWidth, size_t windowHeigth);
      ~Board();

      void draw();
      void setCellValueAt(int i, int j, int value);

      size_t getWidth()const;
      size_t getHeight()const;

      void setUpBoard(int rows, int cols);

   private:
      using GameCells = std::vector<Cell>;

      GameCells m_gameCells;
      size_t m_rows = 0;
      size_t m_cols = 0;

      size_t m_windowWidth;
      size_t m_windowHeight;

   };
}

