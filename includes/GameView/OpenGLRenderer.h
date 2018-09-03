#include "TermRenderer.hpp"
#include "Board.h"

namespace GameView
{
   class OpenGLRenderer :public IGameRenderer
   {
   public:

      OpenGLRenderer(Board* board);

      // Inherited via IGameRenderer
      virtual void Render(const GameState & gameState) const override;

   private:
      void updateBoard(const GameState& gameState) const;

      Board* m_board;

   };


}