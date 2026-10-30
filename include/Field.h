#ifndef FIELD_H
#define FIELD_H

#include <vector>

#include <QGraphicsScene>

class BasicCell;

class Field final : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit Field(QObject* parent = nullptr);
    ~Field() override = default;

    void setFieldSizeCells(int fieldWidthCells, int fieldHeightCells);
    void setMinesCount(int minesCount);

    void generate();
    void generateMinesMap();

private:
    int m_fieldWidthCells{};
    int m_fieldHeightCells{};
    int m_minesCount{};
    std::vector<std::vector<BasicCell*>> m_cellData{};
    std::vector<int> m_minesMap{};
};

#endif //FIELD_H