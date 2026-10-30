#include "Constants.h"
#include "Functions.h"

#include "Field.h"

#include "MinedCell.h"
#include "CleanCell.h"

#include <vector>
#include <random>

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
}

void Field::generateMinesMap()
{
    m_minesMap.clear();
    const int cellsCount{ m_fieldWidthCells * m_fieldHeightCells };

    std::random_device rd{};
    std::seed_seq seed{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
    std::mt19937 generator{ seed };
    std::uniform_int_distribution<int> distribution{ 0, cellsCount };

    while (m_minesMap.size() < m_minesCount)
    {
        if (const int num{ distribution(generator) };
            !contains<int>(m_minesMap, num))
            m_minesMap.push_back(num);
    }
}