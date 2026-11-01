#include "Constants.h"
#include "Functions.h"

#include "Field.h"

#include "MinedCell.h"
#include "CleanCell.h"

#include <vector>
#include <random>
#include <iostream>

#include <QMessageBox>

using size_t = std::size_t;

void Field::leftClickedCellSlot()
{
    if (auto* cell{ dynamic_cast<BasicCell*>(sender()) };
        cell && !cell->isMarked())
    {
        if (!cell->isChecked())
        {
            cell->check();

            if (const auto* cleanCell{ dynamic_cast<CleanCell*>(cell) };
                cleanCell && cleanCell->getMinesAroundCount() == 0)
                checkCellsAround(cleanCell->getCol(), cleanCell->getRow());
        }

        else
            checkCellsAround(cell->getCol(), cell->getRow());
    }
}

void Field::rightClickedCellSlot()
{
    if (auto* cell{ dynamic_cast<BasicCell*>(sender()) };
        cell && !cell->isChecked())
    {
        cell->setMarked(!cell->isMarked());
        m_flagsLeftCount += cell->isMarked() ? -1 : 1;

        if (allMinesMarked())
            victory();
    }
}

void Field::minedCellExplodeSlot()
{
    explode();
}

Field::Field(QObject* parent)
    : QGraphicsScene{ parent }
{
}

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
    m_flagsLeftCount = minesCount;

    m_minesMap.reserve(m_minesCount);
}

void Field::generate()
{
    generateMinesMap();
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
                connect(cell, &MinedCell::rightClickedCellSignal,
                        this, &Field::rightClickedCellSlot);
                connect(cell, &MinedCell::minedCellExplodeSignal,
                        this, &Field::minedCellExplodeSlot);
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
                connect(cell, &CleanCell::rightClickedCellSignal,
                        this, &Field::rightClickedCellSlot);
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

            BasicCell* currCell{ m_cellsData[currColST][currRowST] };

            if (!currCell->isChecked() && !currCell->isMarked())
            {
                currCell->check();
                if (dynamic_cast<MinedCell*>(currCell))
                    return;

                if (auto* currCCell{ dynamic_cast<CleanCell*>(currCell) };
                    currCCell && currCCell->getMinesAroundCount() == 0)
                    checkCellsAround(static_cast<int>(currColST),
                                     static_cast<int>(currRowST));
            }
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

void Field::explode()
{
    for (const int num : m_minesMap)
    {
        const size_t col{ static_cast<size_t>(num / m_fieldHeightCells) };
        const size_t row{ static_cast<size_t>(num % m_fieldHeightCells) };

        m_cellsData[col][row]->BasicCell::check();
    }

    defeat();
}

void Field::victory()
{
    endGame();
}

void Field::defeat()
{
    endGame();
}

void Field::endGame()
{
    m_cellsData.clear();
    m_minesMap.clear();

    emit endGameSignal();
}