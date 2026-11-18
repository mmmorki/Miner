#ifndef CLEANCELL_H
#define CLEANCELL_H

#include "BasicCell.h"

class CleanCell final : public BasicCell
{
    Q_OBJECT

public:
    static inline std::array<QString, 8> numTextures{ ":/textures/numbers/1.png",
        ":/textures/numbers/2.png", ":/textures/numbers/3.png",
        ":/textures/numbers/4.png", ":/textures/numbers/5.png",
        ":/textures/numbers/6.png", ":/textures/numbers/7.png",
        ":/textures/numbers/8.png" };

    explicit CleanCell(int col, int row, QGraphicsItem* parent = nullptr);
    ~CleanCell() override = default;

    [[nodiscard]] int getMinesAroundCount() const;

    void incrementMinesAroundCount();

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
        QWidget* widget = nullptr) override;

private:
    int m_minesAroundCount{ 0 };
};

#endif //CLEANCELL_H