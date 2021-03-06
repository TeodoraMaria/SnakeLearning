#include <SDL.h>
#include <memory>
#include <iostream>

#include "Controller.h"
#include "EventHandler.h"
#include "Board.h"
#include "HumanPlayer2.h"
#include "GameLogic/GameState.h"
#include "AI/HardCoded/SingleBot.hpp"

#include <chrono>

namespace GameView
{


Controller::Controller()
{
   m_players = std::vector<IPlayerPtr>(
   {
      std::make_shared<HumanPlayer2>(),
      
     // std::make_shared<HumanPlayer2>(),
     // std::make_shared<AI::HardCoded::SingleBot>(),
      
      /*std::make_shared<AI::HardCoded::SingleBot>(),
      std::make_shared<AI::HardCoded::SingleBot>(),
      std::make_shared<AI::HardCoded::SingleBot>(),
      std::make_shared<AI::HardCoded::SingleBot>(),
      std::make_shared<AI::HardCoded::SingleBot>(),
      std::make_shared<AI::HardCoded::SingleBot>(),
      std::make_shared<AI::HardCoded::SingleBot>(),
      std::make_shared<AI::HardCoded::SingleBot>()
      */
      //IPlayerPtr(new AI::HardCoded::SingleBot()),
   });

	GameOptions gameOptions;
	{
		gameOptions.boardLength = 25;
		gameOptions.boardWidth = 25;
		gameOptions.numFoods = 40;
	}
   m_game = new Game(gameOptions, m_players);
   m_game->InitGame();
}

Controller::~Controller()
{}

void Controller::processInput(const SDL_Event& currentEvent)
{
   //process for the first player
   if (m_players.size() > 0) {
      processInputPlayer1(currentEvent);
   }
   //process for the second player
   if (m_players.size() > 1) {
      processInputPlayer2(currentEvent);
   }
}
 


void Controller::addBoard(Board * board)
{
   m_board = board;
   const GameState state = m_game->GetGameState();

   const GameBoard gameBoard = state.GetGameBoard();
   m_board->setUpBoard(gameBoard.GetBoardLength(), gameBoard.GetBoardWidth());
}

void Controller::addGraphPlotter(GraphPlotter * graphPlotter)
{
    m_graphPlotter = graphPlotter;
}

GameState Controller::getGameState() const
{
   return m_game->GetGameState();
}

void Controller::updateBoard()
{
   const GameState state=m_game->GetGameState();
   const GameBoard gameBoard = state.GetGameBoard();

   for (int i = 0; i < gameBoard.GetBoardLength(); i++) {
      for (int j = 0; j < gameBoard.GetBoardWidth(); j++) {
         int boardValue = gameBoard[Coordinate(i, j)];

         m_board->setCellValueAt(i, j, boardValue);
      }
   }

   std::vector<Snake> snakes=state.GetSnakes();
   for (const Snake& snake : snakes) {
      for (const Coordinate& coord : snake.GetSnakeBody()) {
         m_board->setCellValueAt(coord.GetX(), coord.GetY(), snake.GetSnakeNumber());
      }
   }
}

void Controller::resetPlayersInput()
{
   for (auto player : m_players) {
      auto humanPlayer = std::dynamic_pointer_cast<HumanPlayer2>(player);
      if (humanPlayer != nullptr) {
         humanPlayer->setDirection(GameViewUtils::InputDirection::DEFAULT);
      }
   }
}

bool Controller::sendActions()
{
   m_currentTime = SDL_GetTicks();
   size_t timeRange = 500;
   if (m_currentTime > m_lastTime + timeRange) {
      GameState state = m_game->GetGameState();
      m_game->RunRound();
      
      auto score = static_cast<int>(m_game->GetAllSnakes().at(0).GetScore());
      if (score > 0) {

      m_graphPlotter->addValue(score);
      }   
      m_lastTime = m_currentTime;
      return true;
   }
   return false;
}
size_t Controller::getAliveSnakes() const
{  
   return m_game->GetLivingSnakes().size();
}
size_t Controller::getCols() const
{
    return m_game->GetGameBoard().GetBoardLength();
}
size_t Controller::getLines() const
{
    return m_game->GetGameBoard().GetBoardWidth();
}
void Controller::processInputPlayer1(const SDL_Event & keyPressed)
{
   auto player1 = dynamic_cast<HumanPlayer2*>(m_players[0].get());
   if (keyPressed.type == SDL_KEYDOWN) {
      switch (keyPressed.key.keysym.sym) {

         // wasd for player1
         case SDLK_w: {
            player1->setDirection(GameViewUtils::InputDirection::UP);
            break;
         }
         case SDLK_s: {
            player1->setDirection(GameViewUtils::InputDirection::DOWN);
            break;
         }
         case SDLK_d: {
            player1->setDirection(GameViewUtils::InputDirection::RIGHT);
            break;
         }
         case SDLK_a: {
            player1->setDirection(GameViewUtils::InputDirection::LEFT);
            break;
         }

      }
   }
}
void Controller::processInputPlayer2(const SDL_Event & keyPressed)
{
   auto player2 = dynamic_cast<HumanPlayer2*>(m_players[1].get());

   if (keyPressed.type == SDL_KEYDOWN) {
      switch (keyPressed.key.keysym.sym) {
         // arrow keys for player2

         case SDLK_UP: {
            player2->setDirection(GameViewUtils::InputDirection::UP);
            break;
         }
         case SDLK_DOWN: {
            player2->setDirection(GameViewUtils::InputDirection::DOWN);
            break;
         }
         case SDLK_RIGHT: {
            player2->setDirection(GameViewUtils::InputDirection::RIGHT);
            break;
         }
         case SDLK_LEFT: {
            player2->setDirection(GameViewUtils::InputDirection::LEFT);
            break;
         }
      }
   }
}
   
}
