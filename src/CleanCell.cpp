#include "Constants.h"

#include "CleanCell.h"

#include <QPainter>

CleanCell::CleanCell(const int col, const int row, QGraphicsItem* parent)
    : BasicCell{ col, row, parent }
{
}

void CleanCell::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
    QWidget* widget)
{
    painter->setPen(Constants::CELL_OUTLINE_COLOR );

    if (!isChecked())
        painter->setBrush(QBrush{ Constants::UNCHECKED_CELL_COLOR });

    else
        painter->setBrush(QBrush{ Constants::CHECKED_CELL_COLOR });

    painter->drawRect(QRectF{ 0.0, 0.0, Constants::CELL_WIDTH_PX,
                                        Constants::CELL_HEIGHT_PX });
}

void CleanCell::mousePressEvent(QGraphicsSceneMouseEvent* event)
{

}