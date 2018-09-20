#include "WallGraphicCell.h"
#include <qpainter.h>
using namespace AppUI;

WallGraphicCell::WallGraphicCell(size_t x, size_t y, size_t width, size_t height):GraphicCell(x,y,width,height)
{
    m_baseColor = QColor(203,132,66);//orange;
}

QPainterPath WallGraphicCell::shape() const
{
    QPainterPath path;

    path.addRect(m_x, m_y, m_width + m_shadowX, m_height + m_shadowY);

    return path;
}

void WallGraphicCell::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{  
    QPainterPath path;

    path.moveTo(m_x,m_y+m_height);
    path.lineTo(m_x + m_shadowX, m_y + m_height + m_shadowY);
    path.lineTo(m_x + m_width + m_shadowX, m_y + m_height + m_shadowY);
    path.lineTo(m_x + m_width + m_shadowX, m_y + m_shadowY);
    path.lineTo(m_x + m_width, m_y);
    path.lineTo(m_x, m_y + m_height);

    painter->setBrush(QColor(98,71,50));
    painter->drawPath(path);
    
    GraphicCell::paint(painter, option, widget);
}
