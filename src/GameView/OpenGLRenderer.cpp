#include "OpenGLRenderer.h"

#include "Board.h"
namespace GameView
{
OpenGLRenderer::OpenGLRenderer(Board* board)
{}

void OpenGLRenderer::Render(const GameState & gameState) const
{
   
   const GameBoard gameBoard = gameState.GetGameBoard();
   m_board->setUpBoard(gameBoard.GetBoardLength(), gameBoard.GetBoardWidth());

   updateBoard(gameState);
   m_board->draw();

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
