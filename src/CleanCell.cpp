#include "Constants.h"

#include "CleanCell.h"

#include <QPainter>

CleanCell::CleanCell(const int col, const int row, QGraphicsItem* parent)
    : BasicCell{ col, row, parent }
{
}

int CleanCell::getMinesAroundCount() const { return m_minesAroundCount; }

void CleanCell::incrementMinesAroundCount() { ++m_minesAroundCount; }

void CleanCell::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
    QWidget* widget)
{
    painter->setPen(QPen{ Constants::CELL_OUTLINE_COLOR, 1 });

    if (!isChecked())
    {
        painter->setBrush(QBrush{ Constants::UNCHECKED_CELL_COLOR });
        painter->drawRect(QRectF{ 0.0, 0.0, Constants::CELL_WIDTH_PX,
                                            Constants::CELL_HEIGHT_PX });
    }

    else
    {
        painter->setBrush(QBrush{ Constants::CHECKED_CELL_COLOR });
        painter->drawRect(QRectF{ 0.0, 0.0, Constants::CELL_WIDTH_PX,
                                          Constants::CELL_HEIGHT_PX });
        if (m_minesAroundCount > 0)
            painter->drawText(20.0, 20.0, QString::number(m_minesAroundCount));
    }

    if (isMarked())
        painter->drawPixmap(5.0, 5.0, QPixmap{ ":/textures/flag_texture.png" });
}