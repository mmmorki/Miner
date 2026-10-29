#include "Constants.h"

#include "BasicCell.h"

#include <QPainter>

BasicCell::BasicCell(const int col, const int row, QGraphicsItem* parent)
    : QGraphicsObject { parent }
    , m_col         { col }
    , m_row         { row }
{
    setAcceptedMouseButtons(Qt::MouseButtons{ Qt::LeftButton, Qt::RightButton });

}

QRectF BasicCell::boundingRect() const
{
    return QRectF{ 0.0, 0.0, Constants::CELL_WIDTH_PX,
                   Constants::CELL_HEIGHT_PX };
}

void BasicCell::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                      QWidget* widget)
{
    painter->setBrush(QBrush{ QColor{ 186, 186, 186 } });
    painter->drawRect(QRectF{ 0.0, 0.0, Constants::CELL_WIDTH_PX,
                              Constants::CELL_HEIGHT_PX });
}