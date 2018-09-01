#include "Controller.h"
#include <SDL.h>
#include <memory>

#include "EventHandler.h"
#include "Board.h"
#include "GameLogic\GameState.h"

namespace GameView
{


Controller::Controller()
{
   std::vector<IPlayerPtr> players(
   {
      std::make_shared<HumanPlayer>()
      //		IPlayerPtr(new AI::HardCoded::SingleBot()),
      //		IPlayerPtr(new AI::HardCoded::SingleBot()),
   });

   const GameOptions gameOptions(GameBoardType::BOX, 20, 20, players.size(), 3);
   m_game=new Game(gameOptions, players);
   m_game->InitGame();

   m_playersDirection = std::vector<InputDirection>(players.size());
}

Controller::Controller(EventHandler * eventHandler)
{
   m_eventHandler = eventHandler;

   std::vector<IPlayerPtr> players(
   {
      std::make_shared<HumanPlayer>()
      //		IPlayerPtr(new AI::HardCoded::SingleBot()),
      //		IPlayerPtr(new AI::HardCoded::SingleBot()),
   });

   const GameOptions gameOptions(GameBoardType::BOX, 20, 20, players.size(), 3);
   m_game = new Game(gameOptions, players);
   m_game->InitGame();

   m_playersDirection = std::vector<InputDirection>(players.size());
}


Controller::~Controller()
{}

void Controller::processInput(const SDL_Event& currentEvent)
{
      if (currentEvent.type == SDL_KEYDOWN) {
         switch (currentEvent.key.keysym.sym) {
            case SDLK_w: {
              // translate(0, 0.2);
               m_game->MoveSnake(11, SnakeMove::FORWARD);
               
               //m_game->PrintBoard();

               m_playersDirection[0] = InputDirection::UP;


               break;
            }
            case SDLK_s: {
              // translate(0, -0.2);
               m_game->MoveSnake(11, SnakeMove::DOWN);
               m_playersDirection[0] = InputDirection::DOWN;
               break;
            }
            case SDLK_d: {
              // translate(0.2, 0);
               m_game->MoveSnake(11, SnakeMove::LEFT);
               m_playersDirection[0] = InputDirection::RIGHT;

               break;
            }
            case SDLK_a: {
               //translate(-0.2, 0);
               m_game->MoveSnake(11, SnakeMove::RIGHT);

               m_playersDirection[0] = InputDirection::LEFT;

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
   switch (m_playersDirection[0]) {
      default:
         break;
   }
}

void Controller::setPlayerDirection()
{
   currentTime = SDL_GetTicks();
   if (currentTime > lastTime + 1000) {
      lastTime = currentTime;

      

   }

}

}
