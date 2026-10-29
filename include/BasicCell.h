#ifndef BASICCELL_H
#define BASICCELL_H

#include <QGraphicsObject>

class BasicCell : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit BasicCell(int col, int row, QGraphicsItem* item = nullptr);
    ~BasicCell() override = default;

protected:
    [[nodiscard]] QRectF boundingRect() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override = 0;

private:
    int m_col{};
    int m_row{};
};

#endif //BASICCELL_H