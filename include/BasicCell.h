#ifndef BASICCELL_H
#define BASICCELL_H

#include <QGraphicsObject>

class BasicCell : public QGraphicsObject
{
    Q_OBJECT

public: signals:
    void leftClickedCellSignal();

    void cellCheckedSignal();

public:
    explicit BasicCell(int col, int row, QGraphicsItem* parent = nullptr);
    ~BasicCell() override = default;

    [[nodiscard]] int getCol() const;
    [[nodiscard]] int getRow() const;
    [[nodiscard]] bool isChecked() const;
    [[nodiscard]] bool isMarked() const;

    void check();
    void mark();
    void unmark();

protected:
    [[nodiscard]] QRectF boundingRect() const override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    int m_col{};
    int m_row{};
    bool m_checked{ false };
    bool m_marked{ false };
};

#endif //BASICCELL_H