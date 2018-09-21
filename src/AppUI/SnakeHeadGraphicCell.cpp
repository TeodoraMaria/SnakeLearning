#include "SnakeHeadGraphicCell.h"
#include <qpainter.h>

using namespace AppUI;

SnakeHeadGraphicCell::SnakeHeadGraphicCell(size_t x, size_t y, size_t width, size_t height, int snakeNumber):
    SnakeGraphicCell(x,y,width,height,snakeNumber)
{
    m_baseColor = QColor(m_baseColor.red() * SHADOW_FACTOR, m_baseColor.green()*SHADOW_FACTOR, m_baseColor.blue()*SHADOW_FACTOR);
}

QPainterPath SnakeHeadGraphicCell::shape() const
{
    return SnakeGraphicCell::shape();
}

void SnakeHeadGraphicCell::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    SnakeGraphicCell::paint(painter,option,widget);
}
