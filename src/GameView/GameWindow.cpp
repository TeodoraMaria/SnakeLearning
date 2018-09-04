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
      m_eventHandler = std::make_unique<EventHandler>();
     // m_eventHandler.reset(new EventHandler());
      m_eventHandler->addGameWindow(this);

      m_controller.reset(new Controller());
      m_eventHandler->addGameController(m_controller.get());

      m_renderer = OpenGLRenderer(screenWidth, screenHeight, m_controller->getLines(), m_controller->getCols());

      m_board.reset(m_renderer.getBoard());
      m_controller->addBoard(m_board.get());
   }

   GameWindow::~GameWindow()
   {}

   
   void GameWindow::run()
   {
     // m_renderer.initWindow();
      gameLoop();
   }

   void GameWindow::processInput()
   {
      m_eventHandler->processInput();
      if (m_controller->sendActions()) {
         m_controller->updateBoard();
      }
   }

   void GameWindow::gameLoop()
   {
      while (m_gameSate != GameState::EXIT) {
         processInput();
         drawGame();
         m_eventHandler->checkIfGameOver();
      }

   }

   void GameWindow::drawGame()
   {
      m_renderer.Render(m_controller->getGameState());
   }
   void GameWindow::exitGame()
   {
      m_gameSate = GameState::EXIT;
   }
}
