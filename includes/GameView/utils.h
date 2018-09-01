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
         BLOCK,
         FIELD,
         FOOD1,
         FOOD2,
         FOOD3,
         SNAKE_HEAD,
         SNAKE_TAIL,
         SNAKE_BODY_HORIZONTAL,
         SNAKE_BODY_VERTICAL,
         SNAKE_BODY_BENT_NE,
         SNAKE_BODY_BENT_SE,
         SNAKE_BODY_BENT_SW,
         SNAKE_BODY_BENT_NW
      };

   }
}