#ifndef FIELD_H
#define FIELD_H

#include "Enums.h"

#include <vector>
#include <queue>

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
    void rightClickedCellSlot();
    void cellCheckedSlot();
    void cellMarkedSlot();
    void cellUnmarkedSlot();

public:
    explicit Field(std::int32_t widthCells, std::int32_t heightCells,
                   std::int32_t minesCount, QObject* parent = nullptr);
    explicit Field(const QByteArray& data, QObject* parent = nullptr);
    ~Field() override = default;

    [[nodiscard]] QByteArray toByteArray() const;

    void clearData();

    void block();
    void unblock();

    [[nodiscard]] bool allMinesMarked() const;

    void defeat();
    void victory();

protected:
    void generate();
    void generateMinesMap();
    void arrangeNums();
    void incrementCellsNumsAround(int col, int row);
    void checkCellsAround(int col, int row);
    void openCellsArea(const BasicCell* startCell);

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

    [[nodiscard]] std::int32_t toMapNum(std::int32_t col, std::int32_t row) const;
    [[nodiscard]] std::pair<std::int32_t, std::int32_t> fromMapNum(
        std::int32_t mapNum) const;

private:
    std::int32_t m_fieldWidthCells{};
    std::int32_t m_fieldHeightCells{};
    std::int32_t m_minesCount{};
    std::int32_t m_cleanCellsLeft{};
    bool m_blocked{ false };
    std::vector<std::vector<BasicCell*>> m_cellsData{};
    std::vector<std::int32_t> m_minesMap{};
    std::vector<std::int32_t> m_checkedMap{};
    std::vector<std::int32_t> m_markedMap{};
    std::queue<BasicCell*> m_cellsToOpen{};
    bool m_openingCells{ false };
};

#endif //FIELD_H