#include <iostream>
#include <string>
#include <vector>

#include <SDL.h>
#include <GL/glew.h>

#include "GameLogic/Game.h"
#include "GameLogic/HumanPlayer.h"

#include "GameWindow.h"
#include "EventHandler.h"




namespace GameView
{

   GameWindow::GameWindow(size_t screenWidth, size_t screenHeight) 
      :m_screenWidth(screenWidth), m_screenHeight(screenHeight)
   {
      m_eventHandler = new EventHandler();
      m_eventHandler->addGameWindow(this);

      m_controller = new Controller();
      m_eventHandler->addGameController(m_controller);

   }

   GameWindow::~GameWindow()
   {}


   static void fatalError(std::string error)
   {
      std::cout << error << std::endl;
      SDL_Quit();
      exit(1);
   }


   void GameWindow::run()
   {
      initSystems();
      gameLoop();
   }


   void GameWindow::initSystems()
   {
      //initial sdl;
      SDL_Init(SDL_INIT_EVERYTHING);

      m_window = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenWidth, SDL_WINDOW_OPENGL);

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
      glOrtho(0.0f, m_screenWidth, m_screenHeight, 0.0f, 0.0f, 1.0f);
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


      m_board = new Board(m_screenWidth, m_screenWidth);

      m_controller->setBoard(m_board);
      //m_board = new Board(m_gameBoard, m_screenWidth, m_screenHeight);
   }

   void GameWindow::processInput()
   {
      m_eventHandler->processInput();
      m_controller->updateBoard(m_board);
   }

   void GameWindow::gameLoop()
   {
      while (m_gameSate != EGameState::EXIT) {
         processInput();
         drawGame();
      }
   }

   void GameWindow::drawGame()
   {
      glClearDepth(1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      m_board->draw();

      SDL_GL_SwapWindow(m_window);

   }
   void GameWindow::exitGame()
   {
      m_gameSate = EGameState::EXIT;
   }
}
