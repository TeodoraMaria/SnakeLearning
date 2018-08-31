#pragma once

#include <vector>

#include "Cell.h"
namespace GameView
{
   class Board
   {
   public:
      Board();
      Board(std::vector<std::vector<int>> board, size_t windowWidth, size_t windowheigth);
      Board(const size_t& rows, const size_t& cols);
      ~Board();

      void consolePrint();
      void draw();

   private:
      using GameCells = std::vector<std::vector<Cell>>;

      GameCells m_gameCells;
      size_t m_rows = 0;
      size_t m_cols = 0;

   };
}

