#include "Constants.h"
#include "Functions.h"

#include "Field.h"

#include "MinedCell.h"
#include "CleanCell.h"

#include <vector>
#include <random>

using size_t = std::size_t;

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
            }

            else
            {
                auto* cell{ new CleanCell{ col, row } };

                addItem(cell);
                cell->setPos(col * Constants::CELL_WIDTH_PX,
                             row * Constants::CELL_HEIGHT_PX);

                currentCol.push_back(cell);
            }

            ++timer;
        }

        m_cellData.push_back(currentCol);
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

        incrementCellsAround(col, row);
    }
}

void Field::incrementCellsAround(const int col, const int row) const
{
    for (int colMod{ -1 }; colMod <= 1; ++colMod)
        for (int rowMod{ -1 }; rowMod <= 1; ++rowMod)
        {
            if (colMod == 0 && rowMod == 0)
                continue;

            const int currCol{ col + colMod };
            const int currRow{ row + rowMod };

            if (currCol >= 0 && currCol < m_fieldWidthCells
                && currRow >= 0 && currRow < m_fieldHeightCells)
            {
                const size_t currColST{ static_cast<size_t>(currCol) };
                const size_t currRowST{ static_cast<size_t>(currRow) };

                BasicCell* currCell{ m_cellData[currColST][currRowST] };


                if (auto* currCCell{ dynamic_cast<CleanCell*>(currCell) };
                    currCCell)
                    currCCell->incrementMinesAroundCount();
            }
        }
}