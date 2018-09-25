#pragma once
#include <qgraphicsitem.h>
#include <qpainterpath.h>

namespace AppUI
{
    class GraphicCell:public QGraphicsPolygonItem
    {
    public:
        GraphicCell(size_t x,size_t y,size_t width,size_t height);
        ~GraphicCell() = default;

        virtual QPainterPath shape()const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;


    protected:
        size_t m_x;
        size_t m_y;
        static size_t m_width;
        static size_t m_height;
        static qreal m_shadowX;
        static qreal m_shadowY;
        QColor m_baseColor;

        static const double SHADOW_FACTOR;
    };

}