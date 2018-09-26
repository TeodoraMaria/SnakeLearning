#include "SnakeGraphicCell.h"
#include <qpainter.h>

using namespace AppUI;

SnakeGraphicCell::SnakeGraphicCell(size_t x, size_t y, size_t width, size_t height, int snakeNumber):GraphicCell(x,y, width,height)
{
    // srand(snakeNumber * 100);
    m_snakeNumber = snakeNumber;
   // m_baseColor = QColor(164,189,70);
    size_t red = ((snakeNumber*100 & 0xC0) >> 6) * 64;
    size_t green = ((snakeNumber*100 & 0x30) >> 4) * 64;
    size_t blue = ((snakeNumber*100 & 0x0C) >> 2) * 64;

    //m_baseColor= QColor(red,green, blue);
    m_baseColor = QColor(red, green, blue);
    // srand(time(0));
}

QPainterPath SnakeGraphicCell::shape() const
{
    QPainterPath path;

    path.addRect(m_x, m_y, m_width + m_shadowX, m_height + m_shadowY);

    return path;
}

void SnakeGraphicCell::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    QPainterPath path;

    path.moveTo(m_x, m_y + m_height);
    path.lineTo(m_x + m_shadowX, m_y + m_height + m_shadowY);
    path.lineTo(m_x + m_width + m_shadowX, m_y + m_height + m_shadowY);
    path.lineTo(m_x + m_width + m_shadowX, m_y + m_shadowY);
    path.lineTo(m_x + m_width, m_y);
    path.lineTo(m_x, m_y + m_height);

    painter->setBrush(QColor(m_baseColor.red() * SHADOW_FACTOR,m_baseColor.green()*SHADOW_FACTOR,m_baseColor.blue()*SHADOW_FACTOR));
    painter->drawPath(path);

    GraphicCell::paint(painter, option, widget);
}
