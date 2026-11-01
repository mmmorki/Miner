#include "Constants.h"

#include "MinedCell.h"

#include <iostream>

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

MinedCell::MinedCell(const int col, const int row, QGraphicsItem* parent)
    : BasicCell{ col, row, parent }
{
}

void MinedCell::check()
{
    BasicCell::check();
    emit minedCellExplodeSignal();
    std::cerr << 122;
}

void MinedCell::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
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
        painter->drawPixmap(5.0, 5.0, QPixmap{ ":/textures/mine_texture.png" });
    }

    if (isMarked())
        painter->drawPixmap(5.0, 5.0, QPixmap{ ":/textures/flag_texture.png" });
}