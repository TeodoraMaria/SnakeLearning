#pragma once
#include "SnakeGraphicCell.h"

namespace AppUI
{
    class SnakeHeadGraphicCell :public SnakeGraphicCell
    {
    public:
        SnakeHeadGraphicCell(size_t x, size_t y, size_t width, size_t height, int snakeNumber);
        ~SnakeHeadGraphicCell() = default;

        virtual QPainterPath shape()const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    };
}