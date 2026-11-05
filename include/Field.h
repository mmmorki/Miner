#ifndef FIELD_H
#define FIELD_H

#include "Enums.h"

#include <vector>

#include <QGraphicsScene>

class BasicCell;
class MinedCell;

class Field final : public QGraphicsScene
{
    Q_OBJECT

public: signals:
    void gameOverVictorySignal();
    void gameOverDefeatSignal();

    void toOnlineLeftClickSignal(std::int32_t col, std::int32_t row);
    void toOnlineRightClickSignal(std::int32_t col, std::int32_t row);

public slots:
    void fromOnlineLeftClickSlot(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);
    void fromOnlineRightClickSlot(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);

private slots:
    void leftClickedCellSlot();

    void cellCheckedSlot();

public:
    explicit Field(QObject* parent = nullptr);
    ~Field() override = default;

    void clearData();

    void block();
    void unblock();

    void setFieldSizeCells(int fieldWidthCells, int fieldHeightCells);
    void setMinesCount(int minesCount);

    void generate();
    void generateMinesMap();

    void arrangeNums() const;
    void incrementCellsNumsAround(int col, int row) const;

    void checkCellsAround(int col, int row) const;

    [[nodiscard]] bool allMinesMarked() const;

    void defeat();
    void victory();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private:
    int m_fieldWidthCells{};
    int m_fieldHeightCells{};
    int m_minesCount{};
    int m_flagsLeft{};
    int m_cleanCellsLeft{};
    bool m_blocked{ false };
    std::vector<std::vector<BasicCell*>> m_cellsData{};
    std::vector<int> m_minesMap{};
};

#endif //FIELD_H