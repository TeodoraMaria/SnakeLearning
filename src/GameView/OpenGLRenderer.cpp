#include "OpenGLRenderer.h"
#include "Board.h"

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


OpenGLRenderer::OpenGLRenderer(size_t resolutionX,size_t resolutionY)
{
    m_board = new Board(resolutionX, resolutionY);

    SDL_Init(SDL_INIT_EVERYTHING);
    const size_t width = m_board->getWidth();
    const size_t height = m_board->getHeight();
    m_window = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

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
    glOrtho(0.0f, width, height, 0.0f, 0.0f, 1.0f);
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

}

OpenGLRenderer::OpenGLRenderer(Board* board):m_board(board)
{}

void OpenGLRenderer::Render(const GameState & gameState) const
{
   glClearDepth(1.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
   const GameBoard gameBoard = gameState.GetGameBoard();
   m_board->setUpBoard(gameBoard.GetBoardLength(), gameBoard.GetBoardWidth());

   updateBoard(gameState);
   m_board->draw();

   SDL_GL_SwapWindow(m_window);

}

Board * OpenGLRenderer::getBoard() const
{
    return m_board;
}


void OpenGLRenderer::initWindow()
{
   SDL_Init(SDL_INIT_EVERYTHING);
   const size_t width=m_board->getWidth();
   const size_t height=m_board->getHeight();
   m_window = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

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
   glOrtho(0.0f, width, height, 0.0f, 0.0f, 1.0f);
   glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
}

void OpenGLRenderer::updateBoard(const GameState & gameState) const
{
   const GameBoard gameBoard = gameState.GetGameBoard();
   for (int i = 0; i < gameBoard.GetBoardLength(); i++) {
      for (int j = 0; j < gameBoard.GetBoardWidth(); j++) {
         int boardValue = gameBoard[Coordinate(i, j)];
         m_board->setCellValueAt(i, j, boardValue);
      }
   }

   std::vector<Snake> snakes = gameState.GetSnakes();
   for (const Snake& snake : snakes) {
      for (const Coordinate& coord : snake.GetSnakeBody()) {
         m_board->setCellValueAt(coord.GetX(), coord.GetY(), snake.GetSnakeNumber());
      }
   }
   

}



}
