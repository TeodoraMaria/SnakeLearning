#pragma once

#include "utils.h"

namespace GameView
{
    class Cell
    {
    public:
        Cell();
        Cell(double x, double y, double width, double height, int value);
        Cell(double x, double y, double width, double height, int value, GameViewUtils::CellType celltype);
        ~Cell();

        void draw() const;

        void drawBlock() const;

        void drawWall() const;
        void drawField() const;
        void drawFood() const;

        void drawSnakeTail() const;
        void drawSnakeHead() const;
        void drawSnakeBody() const;


        void setValue(int value);
        void setCellType(GameViewUtils::CellType cellType);

        void printConsole() const;

    private:
        void setGlColor() const;
        void rotate(double& x, double& y, double degrees) const;

        double m_x = 0;
        double m_y = 0;
        double  m_width = 0;
        double m_height = 0;

        int m_value = 0;

        GameViewUtils::CellType m_cellType;
    };
}
