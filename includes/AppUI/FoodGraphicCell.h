#include "GraphicCell.h"

namespace AppUI
{
    class FoodGraphicCell :public GraphicCell
    {
    public:
        FoodGraphicCell(size_t x, size_t y, size_t width, size_t height);
        ~FoodGraphicCell() = default;

        virtual QPainterPath shape()const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    };
}