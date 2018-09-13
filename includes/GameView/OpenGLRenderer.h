#pragma once

#include "TermRenderer.hpp"
#include "Board.h"
#include "GraphPlotter.h"

struct SDL_Window;


namespace GameView
{
	/*
	** If additional parameters are added to this model, please
	** add them to CofigLoading/OpenGLRendererModelJson.h.
	*/
	
	struct OpenGLRendererModel
	{
		size_t resolutionX = 500;
		size_t resolutionY = 500;
		size_t lines = 10;
		size_t cols = 10;
	};

   class OpenGLRenderer :public IGameRenderer
   {
   public:
      OpenGLRenderer(size_t resolutionX, size_t resolutionY, size_t lines, size_t cols);
      OpenGLRenderer(const OpenGLRendererModel& model);
      OpenGLRenderer();

      // Inherited via IGameRenderer
      virtual void Render(const GameState & gameState) const override;

      Board* getBoard()const; 

      void addGraphPlotter(GraphPlotter* graphPlotter);

   private:
      void updateBoard(const GameState& gameState) const;
      void init() const;

      mutable bool m_initialized = false;
      size_t m_resolutionX;
      size_t m_resolutionY;

      Board* m_board=nullptr;

      mutable SDL_Window* m_window=nullptr;

      GraphPlotter* m_graphPlotter=nullptr;
   };


}
