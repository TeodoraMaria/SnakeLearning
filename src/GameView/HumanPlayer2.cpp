#include "HumanPlayer2.h"
#include <iostream>

namespace GameView
{

HumanPlayer2::HumanPlayer2()
{}


HumanPlayer2::~HumanPlayer2()
{}

SnakeMove HumanPlayer2::GetNextAction(const GameState & gameState)
{
   SnakeMove move = GetUserInput();
   

   TranslateMoveToSnakeView(move, gameState);
   return move != SnakeMove::DOWN ? move : SnakeMove::FORWARD;
}

void HumanPlayer2::setDirection(GameViewUtils::InputDirection direction)
{
   m_direction = direction;
}

SnakeMove HumanPlayer2::GetUserInput() const
{
   switch (m_direction) {
      case GameViewUtils::InputDirection::UP: {
         std::cout << "up"<<std::endl;
         return SnakeMove::FORWARD;
         break;
      }
      case GameViewUtils::InputDirection::DOWN: {
         std::cout << "down" << std::endl;
         return SnakeMove::DOWN;
         break;
      }
      case GameViewUtils::InputDirection::LEFT: {
         std::cout << "left" << std::endl;
         return SnakeMove::RIGHT;
         break;
      }
      case GameViewUtils::InputDirection::RIGHT: {
         std::cout << "right" << std::endl;
         return SnakeMove::LEFT;
         break;
      }
      default: {
         std::cout << "default forward" << std::endl;
         return SnakeMove::FORWARD;
         break;
      }
   }
}

void HumanPlayer2::TranslateMoveToSnakeView(SnakeMove & move, const GameState & gameState) const
{
   const auto& snake = gameState.GetSnake(GetSnakeNumber());
   auto orientation = snake.GetOrientation();
   RotateLeft(move);
   if (orientation == Coordinate::RIGHT) {
      RotateLeft(move);
   } else if (orientation == Coordinate::DOWN) {
      RotateLeft(move);
      RotateLeft(move);
   } else if (orientation == Coordinate::LEFT) {
      RotateLeft(move);
      RotateLeft(move);
      RotateLeft(move);
   }
}

void HumanPlayer2::RotateLeft(SnakeMove & move) const
{
   switch (move) {
      case SnakeMove::FORWARD:
         move = SnakeMove::LEFT;
         return;
      case SnakeMove::LEFT:
         move = SnakeMove::DOWN;
         return;
      case SnakeMove::DOWN:
         move = SnakeMove::RIGHT;
         return;
      case SnakeMove::RIGHT:
         move = SnakeMove::FORWARD;
         return;
   }
}

}
