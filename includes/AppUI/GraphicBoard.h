#pragma once
#include "GameLogic/GameState.h"
#include <qgraphicsscene.h>
#include <memory>


namespace AppUI
{   
    class GraphicCell;

    class GraphicBoard :public QGraphicsScene
    {
    public:
        GraphicBoard(size_t windowWidth, size_t windowHeigth);
        ~GraphicBoard() = default;
        void updateBoard(const GameState & gameState);
    };


}