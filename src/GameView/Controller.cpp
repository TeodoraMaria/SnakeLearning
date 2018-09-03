#include <SDL.h>
#include <memory>
#include <iostream>

#include "Controller.h"
#include "EventHandler.h"
#include "Board.h"
#include "HumanPlayer2.h"
#include "GameLogic/GameState.h"
#include "AI/HardCoded/SingleBot.hpp"

namespace GameView
{


Controller::Controller()
{
   m_players = std::vector<IPlayerPtr>(
   {
      //std::make_shared<HumanPlayer2>(),
      
      //std::make_shared<HumanPlayer2>(),
      std::make_shared<AI::HardCoded::SingleBot>(),
      std::make_shared<AI::HardCoded::SingleBot>(),
      //IPlayerPtr(new AI::HardCoded::SingleBot()),
   });

	GameOptions gameOptions;
	{
		gameOptions.boardLength = 20;
		gameOptions.boardWidth = 20;
		gameOptions.numberOfPlayers = m_players.size();
		gameOptions.foodPortions = 1;
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

bool Controller::sendActions()
{
   m_currentTime = SDL_GetTicks();
   size_t timeRange = 2;
   if (m_currentTime > m_lastTime + timeRange) {
      GameState state = m_game->GetGameState();

      if (m_game->GetLivingSnakes().size() == 1) {
         std::cout << "1";
      }

      for (auto player : m_players) {
         
         const auto snakeNumber = player->GetSnakeNumber();

         const Snake& snake = state.GetSnake(snakeNumber);
         if (snake.GetSnakeSize() != 0) {
            const auto chosenMove = player->GetNextAction(state);
            m_game->MoveSnake(snakeNumber, chosenMove);

            //std::cout << player->GetSnakeNumber()<<" ";
            auto humanPlayer = std::dynamic_pointer_cast<HumanPlayer2>(player);

            if (humanPlayer != nullptr) {
               humanPlayer->setDirection(Utils::InputDirection::DEFAULT);
            }
         }
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
void Controller::processInputPlayer1(const SDL_Event & keyPressed)
{
   auto player1 = dynamic_cast<HumanPlayer2*>(m_players[0].get());
   if (keyPressed.type == SDL_KEYDOWN) {
      switch (keyPressed.key.keysym.sym) {

         // wasd for player1
         case SDLK_w: {
            player1->setDirection(Utils::InputDirection::UP);
            break;
         }
         case SDLK_s: {
            player1->setDirection(Utils::InputDirection::DOWN);
            break;
         }
         case SDLK_d: {
            player1->setDirection(Utils::InputDirection::RIGHT);
            break;
         }
         case SDLK_a: {
            player1->setDirection(Utils::InputDirection::LEFT);
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
            player2->setDirection(Utils::InputDirection::UP);
            break;
         }
         case SDLK_DOWN: {
            player2->setDirection(Utils::InputDirection::DOWN);
            break;
         }
         case SDLK_RIGHT: {
            player2->setDirection(Utils::InputDirection::RIGHT);
            break;
         }
         case SDLK_LEFT: {
            player2->setDirection(Utils::InputDirection::LEFT);
            break;
         }
      }
   }
}
   
}
