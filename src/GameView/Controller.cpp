#include "Controller.h"
#include <SDL.h>
#include <memory>

#include "EventHandler.h"
#include "Board.h"
#include "HumanPlayer2.h"
#include "GameLogic\GameState.h"

namespace GameView
{


Controller::Controller()
{
   
  // m_players.resize(1);

  // m_players.push_back(std::make_shared<HumanPlayer2>());

   m_players = std::vector<IPlayerPtr>({std::make_shared<HumanPlayer2>()});

   std::vector<IPlayerPtr> players(
   {
      std::make_shared<HumanPlayer>()
      //		IPlayerPtr(new AI::HardCoded::SingleBot()),
      //		IPlayerPtr(new AI::HardCoded::SingleBot()),
   });
   
   const GameOptions gameOptions(GameBoardType::BOX, 20, 20, m_players.size(), 3);
   m_game=new Game(gameOptions, m_players);
   m_game->InitGame();

     // m_playersDirection = std::vector<Utils::InputDirection>(m_players.size());
}

Controller::~Controller()
{}

void Controller::processInput(const SDL_Event& currentEvent)
{
   auto player = dynamic_cast<HumanPlayer2*>(m_players[0].get());

      if (currentEvent.type == SDL_KEYDOWN) {
         switch (currentEvent.key.keysym.sym) {
            case SDLK_w: {
               player->setDirection(Utils::InputDirection::UP);
               break;
            }
            case SDLK_s: {
               player->setDirection(Utils::InputDirection::DOWN);
               break;
            }
            case SDLK_d: {
               player->setDirection(Utils::InputDirection::RIGHT);
               break;
            }
            case SDLK_a: {
               player->setDirection(Utils::InputDirection::LEFT);
               break;
            }
         }
      }
}
 

void Controller::setBoard(Board* board)
{
   GameState state = m_game->GetGameState();

   GameBoard gameBoard = state.GetGameBoard();

   board->setUpBoard(gameBoard.GetBoardLength(), gameBoard.GetBoardWidth());

   for (int i = 0; i < gameBoard.GetBoardLength(); i++) {
      for (int j = 0; j < gameBoard.GetBoardWidth(); j++) {
         board->setCellAt(i, j, gameBoard[Coordinate(i, j)]);
      }
   }
}

void Controller::updateBoard(Board* board)
{
   GameState state=m_game->GetGameState();

   GameBoard gameBoard = state.GetGameBoard();

   for (int i = 0; i < gameBoard.GetBoardLength(); i++) {
      for (int j = 0; j < gameBoard.GetBoardWidth(); j++) {
         board->setCellAt(i, j, gameBoard[Coordinate(i, j)]);
         }
      }
}

void Controller::sendActions()
{
   
   m_currentTime = SDL_GetTicks();
   if (m_currentTime > m_lastTime + 250) {
      GameState state = m_game->GetGameState();
      const auto chosenMove = m_players[0]->GetNextAction(state);

      const auto snakeNumber = m_players[0]->GetSnakeNumber();
      m_game->MoveSnake(snakeNumber, chosenMove);
      m_lastTime = m_currentTime;
   }
}

void Controller::setPlayerDirection()
{
   m_currentTime = SDL_GetTicks();
   if (m_currentTime > m_lastTime + 1000) {
      m_lastTime = m_currentTime;

      

   }

}

}
