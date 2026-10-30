#include "Constants.h"

#include "CleanCell.h"

#include <QPainter>

CleanCell::CleanCell(const int col, const int row, QGraphicsItem* parent)
    : BasicCell{ col, row, parent }
{
}

void CleanCell::incrementMinesAroundCount()
{
    ++m_minesAroundCount;
    update();
}

void CleanCell::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
    QWidget* widget)
{
    painter->setPen(QPen{ Constants::CELL_OUTLINE_COLOR, 1 });

    if (!isChecked())
        painter->setBrush(QBrush{ Constants::UNCHECKED_CELL_COLOR });

    else
        painter->setBrush(QBrush{ Constants::CHECKED_CELL_COLOR });

    painter->drawRect(QRectF{ 0.0, 0.0, Constants::CELL_WIDTH_PX,
                                        Constants::CELL_HEIGHT_PX });
    painter->drawText(2, 10, QString::number(m_minesAroundCount));
}