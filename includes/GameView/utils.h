#pragma once

namespace GameView
{
   namespace Utils
   {
      enum class InputDirection
      {
         UP,
         DOWN,
         LEFT,
         RIGHT
      };
      enum class Color
      {
         RED,
         BLUE,
         GREEN,
      };

      enum class CellType
      {
         WALL,
         FIELD,
         FOOD1,
         FOOD2,
         FOOD3,
         SNAKE_HEAD_N,
         SNAKE_HEAD_S,
         SNAKE_HEAD_W,
         SNAKE_HEAD_E,
         SNAKE_TAIL_N,
         SNAKE_TAIL_S,
         SNAKE_TAIL_W,
         SNAKE_TAIL_E,
         SNAKE_BODY_HORIZONTAL,
         SNAKE_BODY_VERTICAL,
         SNAKE_BODY_BENT_NE,
         SNAKE_BODY_BENT_SE,
         SNAKE_BODY_BENT_SW,
         SNAKE_BODY_BENT_NW
      };

      static double ruleOfThree(double value, double envMaxRange, double valueMaxRange)
      {
         return value*envMaxRange / valueMaxRange;
      }

   }
}