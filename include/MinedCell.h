#ifndef MINEDCELL_H
#define MINEDCELL_H

#include "Constants.h"

#include "BasicCell.h"

#include <QPainter>

class MinedCell final : public BasicCell
{
    Q_OBJECT

public:
    explicit MinedCell(int col, int row, QGraphicsItem* parent = nullptr);
    ~MinedCell() override = default;

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
        QWidget* widget = nullptr) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        return;
    }

};

#endif //MINEDCELL_H