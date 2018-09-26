#include "GraphicBoard.h"
#include "WallGraphicCell.h"
#include "FoodGraphicCell.h"
#include "SnakeGraphicCell.h"
#include "SnakeHeadGraphicCell.h"


using namespace AppUI;

GraphicBoard::GraphicBoard(size_t windowWidth, size_t windowHeigth):QGraphicsScene()
{
    setSceneRect(0, 0, windowWidth, windowHeigth);
    setBackgroundBrush(QBrush(QColor(245,205,47)));//yellow 
}

void GraphicBoard::updateBoard(const GameState & gameState)
{
    clear();
   const auto& gameBoard=gameState.GetGameBoard();
    size_t rows = gameBoard.GetBoardLength();
    size_t cols = gameBoard.GetBoardWidth();
    auto width = sceneRect().width();
    size_t widthOffset;
    size_t heightOffset;
    if (rows > cols) {
    widthOffset = width / rows;
   // size_t heightOffset = sceneRect().height() / cols;
    heightOffset = width / rows;
    } else {
    widthOffset = width / cols;
    // size_t heightOffset = sceneRect().height() / cols;
    heightOffset = width / cols;

    }


    for (size_t i = 0; i < cols; i++) {
        for (size_t j = 0; j < rows; j++) {

            auto coord = Coordinate(i, j);

            int boardValue = gameBoard[coord];
            GraphicCell* cell = nullptr;

            switch (boardValue) {
                case 1: {
                    addItem(new WallGraphicCell(i*widthOffset, j*heightOffset, widthOffset, heightOffset));
                    break;
                }
                case 2: {                  
                    addItem(new FoodGraphicCell(i*widthOffset, j*heightOffset, widthOffset, heightOffset));
                    break;
                }
                default: {
                    if (boardValue != 0) {
                        if (gameState.GetSnake(boardValue).GetSnakeHead() == coord) {
                            addItem(new SnakeHeadGraphicCell(i*widthOffset, j*heightOffset, widthOffset, heightOffset, boardValue));
                        } else {
                            addItem(new SnakeGraphicCell(i*widthOffset, j*heightOffset, widthOffset, heightOffset, boardValue));

                        }
                    }
                    break;
                }                    
            }
        }
    }
}
