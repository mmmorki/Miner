#ifndef CLEANCELL_H
#define CLEANCELL_H

#include "BasicCell.h"

class CleanCell final : public BasicCell
{
    Q_OBJECT

public:
    explicit CleanCell(int col, int row, QGraphicsItem* parent = nullptr);
    ~CleanCell() override = default;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        return;
    }
};

#endif //CLEANCELL_H