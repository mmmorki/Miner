#include "Constants.h"
#include "Functions.h"

#include "Field.h"

#include "MinedCell.h"
#include "CleanCell.h"

#include <vector>
#include <random>
#include <iostream>

#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>

using size_t = std::size_t;

void Field::leftClickedCellSlot()
{
    auto* basicCell{ dynamic_cast<BasicCell*>(sender()) };
    emit toOnlineLeftClickSignal(basicCell->getCol(), basicCell->getRow());

    if (auto* const cleanCell{ dynamic_cast<CleanCell*>(sender()) };
        cleanCell)
    {
        if (cleanCell->isChecked())
            checkCellsAround(cleanCell->getCol(), cleanCell->getRow());

        else
            cleanCell->check();
    }

    else if (auto* const minedCell{ dynamic_cast<MinedCell*>(sender()) };
        minedCell)
        minedCell->check();
}

void Field::fromOnlineLeftClickSlot(const std::int8_t playerNum,
    const std::int32_t col, const std::int32_t row)
{
    const size_t colST{ static_cast<size_t>(col) };
    const size_t rowST{ static_cast<size_t>(row) };

    BasicCell* cell{ m_cellsData[colST][rowST] };
    cell->leftClickedCellSignal();
}

void Field::fromOnlineRightClickSlot(const std::int8_t playerNum,
    const std::int32_t col, const std::int32_t row)
{
    const size_t colST{ static_cast<size_t>(col) };
    const size_t rowST{ static_cast<size_t>(row) };

    if (BasicCell* const cell{ m_cellsData[colST][rowST] };
        cell->isMarked())
        cell->unmark();

    else
        cell->mark();
}

void Field::cellCheckedSlot()
{
    if (const auto* const cleanCell{ dynamic_cast<CleanCell*>(sender()) };
        cleanCell)
    {
        if (--m_cleanCellsLeft == 0)
            victory();

        if (cleanCell->getMinesAroundCount() == 0)
            checkCellsAround(cleanCell->getCol(), cleanCell->getRow());
    }

    else if (const auto* const minedCell{ dynamic_cast<MinedCell*>(sender()) };
        minedCell)
        defeat();
}

Field::Field(QObject* parent)
    : QGraphicsScene{ parent }
{
}

void Field::clearData()
{
    m_cellsData.clear();
    m_minesMap.clear();
}

void Field::block() { m_blocked = true; }

void Field::unblock() { m_blocked = false; }

void Field::setFieldSizeCells(const int fieldWidthCells,
                              const int fieldHeightCells)
{
    m_fieldWidthCells = fieldWidthCells;
    m_fieldHeightCells = fieldHeightCells;

    const qreal fieldWidthPx{ m_fieldWidthCells * Constants::CELL_WIDTH_PX };
    const qreal fieldHeightPx{ m_fieldHeightCells * Constants::CELL_HEIGHT_PX };
    setSceneRect(0, 0, fieldWidthPx, fieldHeightPx);
}

void Field::setMinesCount(const int minesCount)
{
    m_minesCount = minesCount;
    m_flagsLeft = minesCount;

    m_minesMap.reserve(m_minesCount);
}

void Field::generate()
{
    clearData();
    generateMinesMap();

    m_cleanCellsLeft = m_fieldWidthCells * m_fieldHeightCells - m_minesCount;

    int timer{ 0 };

    for (int col{ 0 }; col < m_fieldWidthCells; ++col)
    {
        std::vector<BasicCell*> currentCol{};

        for (int row{ 0 }; row < m_fieldHeightCells; ++row)
        {
            if (contains<int>(m_minesMap, timer))
            {
                auto* cell{ new MinedCell{ col, row } };

                addItem(cell);
                cell->setPos(col * Constants::CELL_WIDTH_PX,
                             row * Constants::CELL_HEIGHT_PX);
                currentCol.push_back(cell);
                connect(cell, &MinedCell::leftClickedCellSignal,
                        this, &Field::leftClickedCellSlot);
                connect(cell, &MinedCell::cellCheckedSignal,
                        this, &Field::cellCheckedSlot);
            }

            else
            {
                auto* cell{ new CleanCell{ col, row } };

                addItem(cell);
                cell->setPos(col * Constants::CELL_WIDTH_PX,
                             row * Constants::CELL_HEIGHT_PX);
                currentCol.push_back(cell);
                connect(cell, &CleanCell::leftClickedCellSignal,
                        this, &Field::leftClickedCellSlot);
                connect(cell, &CleanCell::cellCheckedSignal,
                        this, &Field::cellCheckedSlot);
            }

            ++timer;
        }

        m_cellsData.push_back(currentCol);
    }

    arrangeNums();
}

void Field::generateMinesMap()
{
    m_minesMap.clear();
    const int cellsCount{ m_fieldWidthCells * m_fieldHeightCells };

    std::random_device rd{};
    std::seed_seq seed{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
    std::mt19937 generator{ seed };
    std::uniform_int_distribution distribution{ 0, cellsCount };

    while (m_minesMap.size() < m_minesCount)
    {
        if (const int num{ distribution(generator) };
            !contains<int>(m_minesMap, num))
            m_minesMap.push_back(num);
    }
}

void Field::arrangeNums() const
{
    for (const int num : m_minesMap)
    {
        const int col{ num / m_fieldHeightCells };
        const int row{ num % m_fieldHeightCells };

        incrementCellsNumsAround(col, row);
    }
}

void Field::incrementCellsNumsAround(const int col, const int row) const
{
    for (int colMod{ -1 }; colMod <= 1; ++colMod)
        for (int rowMod{ -1 }; rowMod <= 1; ++rowMod)
        {
            if (colMod == 0 && rowMod == 0)
                continue;

            if (col + colMod < 0 || col + colMod >= m_fieldWidthCells
                || row + rowMod < 0 || row + rowMod >= m_fieldHeightCells)
                continue;

            const size_t currColST{ static_cast<size_t>(col + colMod) };
            const size_t currRowST{ static_cast<size_t>(row + rowMod) };

            BasicCell* currCell{ m_cellsData[currColST][currRowST] };

            if (auto* currCCell{ dynamic_cast<CleanCell*>(currCell) };
                currCCell)
                currCCell->incrementMinesAroundCount();
        }
}

void Field::checkCellsAround(const int col, const int row) const
{
    for (int colMod{ -1 }; colMod <= 1; ++colMod)
        for (int rowMod{ -1 }; rowMod <= 1; ++rowMod)
        {
            if (colMod == 0 && rowMod == 0)
                continue;

            if (col + colMod < 0 || col + colMod >= m_fieldWidthCells
                || row + rowMod < 0 || row + rowMod >= m_fieldHeightCells)
                continue;

            const size_t currColST{ static_cast<size_t>(col + colMod) };
            const size_t currRowST{ static_cast<size_t>(row + rowMod) };

            m_cellsData[currColST][currRowST]->check();
        }
}

bool Field::allMinesMarked() const
{
    bool marked{ true };

    for (const int num : m_minesMap)
    {
        const size_t col{ static_cast<size_t>(num / m_fieldHeightCells) };
        const size_t row{ static_cast<size_t>(num % m_fieldHeightCells) };

        marked &= m_cellsData[col][row]->isMarked();
    }

    return marked;
}

void Field::defeat()
{
    for (const int num : m_minesMap)
    {
        const size_t col{ static_cast<size_t>(num / m_fieldHeightCells) };
        const size_t row{ static_cast<size_t>(num % m_fieldHeightCells) };

        m_cellsData[col][row]->check();
    }

    block();
    emit gameOverDefeatSignal();
}

void Field::victory()
{
    block();
    emit gameOverVictorySignal();
}

void Field::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_blocked)
        event->accept();

    else
        QGraphicsScene::mousePressEvent(event);
}


//Двойное нажатие запрещено
void Field::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    event->accept();
}