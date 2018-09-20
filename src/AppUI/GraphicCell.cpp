#include "GraphicCell.h"
#include "qpen.h"
#include "qpainter.h"
#include "qwidget.h"

using namespace AppUI;

size_t GraphicCell::m_width;
size_t GraphicCell::m_height;
qreal GraphicCell::m_shadowX;
qreal GraphicCell::m_shadowY;

GraphicCell::GraphicCell(size_t x, size_t y, size_t width, size_t height):QGraphicsPolygonItem()
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height; 
    m_shadowX = m_width / 5;
    m_shadowY = m_height / 5;
    m_baseColor = Qt::green;
}

QPainterPath GraphicCell::shape() const
{
    QPainterPath path;
    path.addRect(m_x, m_y, m_width, m_height);

    return path;
}

void GraphicCell::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{   
    painter->setBrush(m_baseColor);
    //painter->setPen(Qt::NoPen);
    painter->drawRect(m_x, m_y, m_width, m_height);
}
