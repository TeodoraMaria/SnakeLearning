#include "TermRenderer.hpp"
#include "Board.h"
#include "SDL.h"

namespace GameView
{
   class OpenGLRenderer :public IGameRenderer
   {
   public:
      OpenGLRenderer(size_t resolutionX, size_t resolutionY, size_t lines, size_t cols);
      OpenGLRenderer();

      // Inherited via IGameRenderer
      virtual void Render(const GameState & gameState) const override;

      Board* getBoard()const; 
   private:
      void updateBoard(const GameState& gameState) const;

      Board* m_board=nullptr;

      SDL_Window* m_window=nullptr;


   };


}