#ifndef FIELD_H
#define FIELD_H

#include <QGraphicsScene>

class Field final : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit Field(QObject* parent = nullptr);
    ~Field() override = default;

    void setFieldSizeCells(int fieldWidthCells, int fieldHeightCells);
    void setMinesCount(int minesCount);

private:
    int m_fieldWidthCells{};
    int m_fieldHeightCells{};
    int m_minesCount{};
};

#endif //FIELD_H