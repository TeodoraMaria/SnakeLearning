#include <SDL.h>
#include <memory>
#include <iostream>

#include "Controller.h"
#include "EventHandler.h"
#include "Board.h"
#include "HumanPlayer2.h"
#include "GameLogic\GameState.h"

namespace GameView
{


Controller::Controller()
{
   m_players = std::vector<IPlayerPtr>(
   {
      std::make_shared<HumanPlayer2>(),
     // std::make_shared<HumanPlayer2>()
   });

   std::vector<IPlayerPtr> players(
   {
      std::make_shared<HumanPlayer>()
      //		IPlayerPtr(new AI::HardCoded::SingleBot()),
      //		IPlayerPtr(new AI::HardCoded::SingleBot()),
   });
   
   const GameOptions gameOptions(GameBoardType::BOX, 20, 20, m_players.size(), 3);

   m_game = std::make_shared<Game>(Game(gameOptions, m_players));
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
 

void Controller::setBoard(Board* board)
{
   GameState state = m_game->GetGameState();

   GameBoard gameBoard = state.GetGameBoard();

   board->setUpBoard(gameBoard.GetBoardLength(), gameBoard.GetBoardWidth());

   std::vector<Snake> snakes = state.GetSnakes();

   for (int i = 0; i < gameBoard.GetBoardLength(); i++) {
      for (int j = 0; j < gameBoard.GetBoardWidth(); j++) {
         board->setCellAt(i, j, gameBoard[Coordinate(i, j)]);
      }
   }
   for (Snake snake : snakes) {
      for (Coordinate coord : snake.GetSnakeBody()) {
         board->setCellAt(coord.GetX(), coord.GetY(), snake.GetSnakeNumber());
      }
   }
}

void Controller::updateBoard(Board* board)
{
   GameState state=m_game->GetGameState();

   GameBoard gameBoard = state.GetGameBoard();

   std::vector<Snake> snakes=state.GetSnakes();

   for (int i = 0; i < gameBoard.GetBoardLength(); i++) {
      for (int j = 0; j < gameBoard.GetBoardWidth(); j++) {
         board->setCellAt(i, j, gameBoard[Coordinate(i, j)]);
      }
   }

   for (Snake snake : snakes) {
      for (Coordinate coord : snake.GetSnakeBody()) {
         board->setCellAt(coord.GetX(), coord.GetY(), snake.GetSnakeNumber());
      }
   }
}

bool Controller::sendActions()
{
   m_currentTime = SDL_GetTicks();
   if (m_currentTime > m_lastTime + 1000) {
      GameState state = m_game->GetGameState();

      for (auto& player : m_players) {
         const auto chosenMove = player->GetNextAction(state);
         const auto snakeNumber = player->GetSnakeNumber();
         m_game->MoveSnake(snakeNumber, chosenMove);
      }
      m_lastTime = m_currentTime;
      return true;
   }
   return false;
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
