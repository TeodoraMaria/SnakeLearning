#include "TermRenderer.hpp"
#include "Board.h"
#include "SDL.h"

namespace GameView
{
   class OpenGLRenderer :public IGameRenderer
   {
   public:
      OpenGLRenderer();
      OpenGLRenderer(Board* board);

      // Inherited via IGameRenderer
      virtual void Render(const GameState & gameState) const override;

      void initWindow();

   private:
      void updateBoard(const GameState& gameState) const;

      Board* m_board;

      SDL_Window* m_window;


   };


}