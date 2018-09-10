#include "TermRenderer.hpp"
#include "Board.h"

struct SDL_Window;

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
      void init() const;

      mutable bool m_initialized = false;
      size_t m_resolutionX;
      size_t m_resolutionY;

      Board* m_board=nullptr;

      mutable SDL_Window* m_window=nullptr;
   };


}
