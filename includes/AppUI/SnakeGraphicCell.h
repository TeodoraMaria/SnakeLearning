#pragma once
#include "GraphicCell.h"

namespace AppUI
{
    class SnakeGraphicCell :public GraphicCell
    {
    public:
        SnakeGraphicCell(size_t x, size_t y, size_t width, size_t height, int snakeNumber);
        ~SnakeGraphicCell() = default;

        virtual QPainterPath shape()const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    private:
        int m_snakeNumber;

    };
}