#ifndef MINEDCELL_H
#define MINEDCELL_H

#include "BasicCell.h"

class MinedCell final : public BasicCell
{
    Q_OBJECT

public: signals:
    void minedCellExplodeSignal();

public:
    explicit MinedCell(int col, int row, QGraphicsItem* parent = nullptr);
    ~MinedCell() override = default;

    void check() override;

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
        QWidget* widget = nullptr) override;
};

#endif //MINEDCELL_H