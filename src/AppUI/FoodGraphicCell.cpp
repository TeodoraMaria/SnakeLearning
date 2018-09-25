#include "FoodGraphicCell.h"
#include <qpainter.h>

using namespace AppUI;

FoodGraphicCell::FoodGraphicCell(size_t x, size_t y, size_t width, size_t height):GraphicCell(x,y,width,height)
{
    m_baseColor = QColor(220, 20, 60);
}

QPainterPath FoodGraphicCell::shape() const
{
    QPainterPath path;
    path.addRect(m_x, m_y, m_width + m_shadowX, m_height + m_shadowY);
    return path;
}

void FoodGraphicCell::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{

    QPainterPath path;

    qreal offsetX = m_width / 3;
    qreal offsetY = m_height / 3;

    qreal newX = m_x + offsetX;
    qreal newY = m_y + offsetY;
    qreal newWidth = m_width - offsetX;
    qreal newHeight = m_height - offsetY;

    path.moveTo(newX, newY + newHeight);
    path.lineTo(newX + m_shadowX, newY + newHeight + m_shadowY);
    path.lineTo(newX + newWidth + m_shadowX, newY + newHeight + m_shadowY);
    path.lineTo(newX + newWidth + m_shadowX, newY + m_shadowY);
    path.lineTo(newX + newWidth, newY);
    path.lineTo(newX, newY + newHeight);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(m_baseColor.red() * SHADOW_FACTOR, m_baseColor.green()*SHADOW_FACTOR, m_baseColor.blue()*SHADOW_FACTOR));
    painter->drawPath(path);
    
    painter->setBrush(m_baseColor);
    painter->drawRect(m_x + offsetX, m_y + offsetY, m_width - offsetX, m_height - offsetY);

    qreal centerX = newWidth / 2;

    qreal drawingPortion = m_width / 8;

    painter->setBrush(QColor(Qt::black));
    painter->drawRect(newX+centerX, newY+2*drawingPortion, 2*drawingPortion, -5* drawingPortion);

    painter->setBrush(QColor(232,186,199));
    painter->drawRect(newX + drawingPortion, newY + drawingPortion, 3*drawingPortion, 3*drawingPortion);

}
