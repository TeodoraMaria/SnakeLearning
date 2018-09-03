#pragma once
#include <vector>
#include <SDL.h>
#include <memory>


#include "GameLogic/Game.h"
#include "GameLogic/HumanPlayer.h"

#include "HumanPlayer2.h"
#include "GameLogic/IPlayer.h"

namespace GameView
{

class Board;
class EventHandler;

class Controller
{
public:
   Controller();
   ~Controller();

   void processInput(const SDL_Event& keyPressed);
   void updateBoard();
   void addBoard(Board* board);

   bool sendActions();

   size_t getAliveSnakes() const;

private:
   void processInputPlayer1(const SDL_Event& keyPressed);
   void processInputPlayer2(const SDL_Event& keyPressed);
   std::vector<IPlayerPtr> m_players;

   //std::shared_ptr<Game> m_game;
   //std::shared_ptr<Board> m_board;
   Game* m_game;
   Board* m_board;

   size_t m_lastTime = 0;
   size_t m_currentTime;
};

}
