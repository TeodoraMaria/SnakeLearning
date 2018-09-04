#include "TermRenderer.hpp"
#include "Board.h"
#include "SDL.h"

namespace GameView
{
   class OpenGLRenderer :public IGameRenderer
   {
   public:
      OpenGLRenderer(size_t resolutionX, size_t resolutionY);
      OpenGLRenderer(Board* board);

      // Inherited via IGameRenderer
      virtual void Render(const GameState & gameState) const override;

      Board* getBoard()const; 

      void initWindow();

   private:
      void updateBoard(const GameState& gameState) const;

      Board* m_board=nullptr;

      SDL_Window* m_window;


   };


}