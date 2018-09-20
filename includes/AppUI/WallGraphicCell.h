#pragma once
#include "GraphicCell.h"

namespace AppUI
{
    class WallGraphicCell :public GraphicCell
    {
    public:
        WallGraphicCell(size_t x, size_t y, size_t width, size_t height);
        virtual QPainterPath shape()const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    };
}