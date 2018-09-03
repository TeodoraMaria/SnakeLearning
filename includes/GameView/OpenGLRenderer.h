#include "TermRenderer.hpp"

namespace GameView
{
   class OpenGLRendered :public IGameRenderer
   {
      // Inherited via IGameRenderer
      virtual void Render(const GameState & gameState) const override;
   };


}