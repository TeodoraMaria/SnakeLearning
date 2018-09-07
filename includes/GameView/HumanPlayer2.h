#pragma once

#include "GameLogic/IPlayer.h"

#include "utils.h"

namespace GameView
{
class HumanPlayer2:public IPlayer
{
public:
   HumanPlayer2();
   ~HumanPlayer2();

   // Inherited via IPlayer
   virtual SnakeMove GetNextAction(const GameState & gameState) override;

   void setDirection(Utils::InputDirection direction);

private:
   SnakeMove GetUserInput() const;
   void TranslateMoveToSnakeView(SnakeMove& move, const GameState& gameState) const;
   void RotateLeft(SnakeMove& move) const;

   //SnakeMove getSnakeMoveRelativeToInput(const GameState& gameState)const;

   Utils::InputDirection m_direction=Utils::InputDirection::DEFAULT;
};

}

