#include "Constants.h"
#include "Functions.h"

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
        {
            const size_t colorIndex{ static_cast<size_t>(m_minesAroundCount - 1) };
            painter->drawPixmap(5.0, 5.0, getCachedPixmap(numFileNames[colorIndex]));
        }
    }

    if (isMarked())
        painter->drawPixmap(5.0, 5.0, QPixmap{ ":/textures/flag_texture.png" });
}