#include "OpenGLRenderer.h"
#include "Board.h"
#include "Utils/MathUtils.h"
#include "GameView/GraphPlotter.h"

#include <SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <string>
namespace GameView
{

void fatalError(std::string error)
{
    std::cout << error << std::endl;
    SDL_Quit();
    exit(1);
}


OpenGLRenderer::OpenGLRenderer(size_t resolutionX,size_t resolutionY,size_t lines,size_t cols)
    :m_resolutionX(resolutionX),m_resolutionY(resolutionY)
{
    m_board = new Board(resolutionX, resolutionY);
    m_board->setUpBoard(lines, cols);
}

OpenGLRenderer::OpenGLRenderer(const OpenGLRendererModel& model) :
	OpenGLRenderer(
		model.resolutionX,
		model.resolutionY,
		model.lines,
		model.cols)
{
}

OpenGLRenderer::OpenGLRenderer()
{}

void OpenGLRenderer::Render(const GameState & gameState) const
{
    if (m_initialized == false) {
        init();
        m_initialized = true;
    }

   glClearDepth(1.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
   const GameBoard gameBoard = gameState.GetGameBoard();
   
   updateBoard(gameState);
   
   m_board->draw();

   if (m_graphPlotter != nullptr) {
      m_graphPlotter->plot();
   }
   SDL_GL_SwapWindow(m_window);
}

Board * OpenGLRenderer::getBoard() const
{
    return m_board;
}

void OpenGLRenderer::addGraphPlotter(GraphPlotter * graphPlotter)
{
    m_graphPlotter = graphPlotter;
}

void OpenGLRenderer::updateBoard(const GameState & gameState) const
{
   const GameBoard& gameBoard = gameState.GetGameBoard();
   for (int i = 0; i < gameBoard.GetBoardLength(); i++) {
      for (int j = 0; j < gameBoard.GetBoardWidth(); j++) {
         int boardValue = gameBoard[Coordinate(i, j)];
         m_board->setCellValueAt(i, j, boardValue);
      }
   }
   /*
   const std::vector<Snake>& snakes = gameState.GetSnakes();
   for (const Snake& snake : snakes) {
      for (const Coordinate& coord : snake.GetSnakeBody()) {
         m_board->setCellValueAt(coord.GetX(), coord.GetY(), snake.GetSnakeNumber());
      }
   }
   */
}

void OpenGLRenderer::init() const
{
    SDL_Init(SDL_INIT_EVERYTHING);
    m_window = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_resolutionX, m_resolutionY, SDL_WINDOW_OPENGL);

    if (m_window == nullptr) {
        fatalError("could not create window");
    }
    SDL_GLContext glContext = SDL_GL_CreateContext(m_window);

    if (glContext == nullptr) {
        fatalError("could not create context");
    }

    GLenum error = glewInit();

    if (error != GLEW_OK) {
        fatalError("could not init gl");
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, m_resolutionX, m_resolutionY, 0.0f, 0.0f, 1.0f);
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
}

}
