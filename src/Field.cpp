#include "Constants.h"
#include "Functions.h"

#include "Field.h"

#include "MinedCell.h"
#include "CleanCell.h"

#include <vector>
#include <random>
#include <iostream>
#include <algorithm>

#include "qiodevice.h"
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>

using std::int8_t;
using std::int32_t;
using std::size_t;

void Field::leftClickedCellSlot()
{
    auto* const basicCell{ dynamic_cast<BasicCell*>(sender()) };
    emit toOnlineLeftClickSignal(basicCell->getCol(), basicCell->getRow());

    if (auto* const cleanCell{ dynamic_cast<CleanCell*>(basicCell) };
        cleanCell)
    {
        if (cleanCell->isChecked())
            openCellsArea(cleanCell);

        else
            cleanCell->check();
    }

    else if (auto* const minedCell{ dynamic_cast<MinedCell*>(sender()) };
        minedCell)
        minedCell->check();
}

void Field::rightClickedCellSlot()
{
    auto* const basicCell{ static_cast<BasicCell*>(sender()) };
    emit toOnlineRightClickSignal(basicCell->getCol(), basicCell->getRow());

    if (basicCell->isMarked())
        basicCell->unmark();

    else
        basicCell->mark();
}

void Field::fromOnlineLeftClickSlot(const int8_t playerNum,
    const int32_t col, const int32_t row)
{
    const size_t colST{ static_cast<size_t>(col) };
    const size_t rowST{ static_cast<size_t>(row) };

    auto* const basicCell{ m_cellsData[colST][rowST] };

    if (auto* const cleanCell{ dynamic_cast<CleanCell*>(basicCell) };
        cleanCell)
    {
        if (cleanCell->isChecked())
            openCellsArea(cleanCell);

        else
            cleanCell->check();
    }

    else if (auto* const minedCell{ dynamic_cast<MinedCell*>(basicCell) };
        minedCell)
        minedCell->check();
}

void Field::fromOnlineRightClickSlot(const int8_t playerNum,
    const int32_t col, const int32_t row)
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
    auto* const cell{ static_cast<BasicCell*>(sender()) };
    const int32_t col{ cell->getCol() };
    const int32_t row{ cell->getRow() };

    if (auto* const cleanCell{ dynamic_cast<CleanCell*>(cell) };
        cleanCell)
    {
        if (--m_cleanCellsLeft == 0)
            victory();

        if (cleanCell->getMinesAroundCount() == 0)
        {
            if (!m_openingCells)
                openCellsArea(cleanCell);

            else
                checkCellsAround(col, row);
        }
    }

    else if (const auto* const minedCell{ dynamic_cast<const MinedCell*>(cell) };
        minedCell)
        defeat();

    m_checkedMap.push_back(toMapNum(col, row));
}

void Field::cellMarkedSlot()
{
    const auto* const cell{ static_cast<const BasicCell*>(sender()) };
    const int32_t col{ cell->getCol() };
    const int32_t row{ cell->getRow() };

    m_markedMap.push_back(toMapNum(col, row));
}

void Field::cellUnmarkedSlot()
{
    const auto* const cell{ static_cast<const BasicCell*>(sender()) };
    const int32_t col{ cell->getCol() };
    const int32_t row{ cell->getRow() };

    const auto iter{ std::find(m_markedMap.begin(), m_markedMap.end(),
                               toMapNum(col, row)) };
    if (iter != m_markedMap.end())
        m_markedMap.erase(iter);
}

Field::Field(const int32_t widthCells, const int32_t heightCells,
             const int32_t minesCount, QObject* parent)
    : QGraphicsScene{ parent }
    , m_fieldWidthCells{ widthCells }
    , m_fieldHeightCells{ heightCells }
    , m_minesCount{ minesCount }
    , m_cleanCellsLeft{ widthCells * heightCells - minesCount }
{
    generateMinesMap();
    generate();
}

Field::Field(const QByteArray& data, QObject* parent)
    : QGraphicsScene{ parent }
{
    QDataStream dataStream{ data };
    dataStream.setByteOrder(QDataStream::BigEndian);
    dataStream.setVersion(QDataStream::Qt_6_10);

    dataStream >> m_fieldWidthCells >> m_fieldHeightCells;

    int32_t minesMapLength{};
    dataStream >> minesMapLength;
    for (int32_t bytes{ 0 }; bytes < minesMapLength; bytes += 4)
    {
        int32_t num{};
        dataStream >> num;
        m_minesMap.push_back(num);
    }

    m_minesCount = static_cast<int32_t>(m_minesMap.size());
    m_cleanCellsLeft = m_fieldWidthCells * m_fieldHeightCells - m_minesCount;

    int32_t checkedMapLength{};
    dataStream >> checkedMapLength;
    for (int32_t bytes{ 0 }; bytes < checkedMapLength; bytes += 4)
    {
        int32_t num{};
        dataStream >> num;
        m_checkedMap.push_back(num);
    }

    int32_t markedMapLength{};
    dataStream >> markedMapLength;
    for (int32_t bytes{ 0 }; bytes < markedMapLength; bytes += 4)
    {
        int32_t num{};
        dataStream >> num;
        m_markedMap.push_back(num);
    }

    generate();

    int32_t timer{ 0 };

    for (int32_t col{ 0 }; col < m_fieldWidthCells; ++col)
        for (int32_t row{ 0 }; row < m_fieldHeightCells; ++row)
        {
            size_t colST{ static_cast<size_t>(col) };
            size_t rowST{ static_cast<size_t>(row) };

            if (contains<int>(m_checkedMap, timer))
                m_cellsData[col][row]->check();

            if (contains<int>(m_markedMap, timer))
                m_cellsData[col][row]->mark();

            ++timer;
        }
}

void Field::clearData()
{
    for (const std::vector<BasicCell*>& col : m_cellsData)
        for (BasicCell* cell : col)
            cell->deleteLater();

    m_cellsData.clear();

    m_minesMap.clear();
    m_checkedMap.clear();
    m_markedMap.clear();
}

void Field::block() { m_blocked = true; }

void Field::unblock() { m_blocked = false; }

void Field::generate()
{
    int32_t timer{ 0 };

    for (int32_t col{ 0 }; col < m_fieldWidthCells; ++col)
    {
        std::vector<BasicCell*> currentCol{};

        for (int32_t row{ 0 }; row < m_fieldHeightCells; ++row)
        {
            BasicCell* cell{ nullptr };

            if (contains<int>(m_minesMap, timer))
                cell = new MinedCell{ col, row };

            else
                cell = new CleanCell{ col, row };

            addItem(cell);
            currentCol.push_back(cell);
            cell->setPos(col * Constants::CELL_WIDTH_PX,
                         row * Constants::CELL_HEIGHT_PX);

            connect(cell, &BasicCell::leftClickedCellSignal,
                    this, &Field::leftClickedCellSlot);
            connect(cell, &BasicCell::rightClickedCellSignal,
                    this, &Field::rightClickedCellSlot);
            connect(cell, &BasicCell::cellCheckedSignal,
                    this, &Field::cellCheckedSlot);
            connect(cell, &BasicCell::cellMarkedSignal,
                    this, &Field::cellMarkedSlot);
            connect(cell, &BasicCell::cellUnmarkedSignal,
                    this, &Field::cellUnmarkedSlot);

            std::cerr << ++timer << std::endl;
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
    std::uniform_int_distribution distribution{ 0, cellsCount - 1 };

    while (m_minesMap.size() < m_minesCount)
    {
        if (const int num{ distribution(generator) };
            !contains<int>(m_minesMap, num))
            m_minesMap.push_back(num);
    }
}

void Field::arrangeNums()
{
    for (const int num : m_minesMap)
    {
        const auto [col, row] { fromMapNum(num) };
        incrementCellsNumsAround(col, row);
    }
}

void Field::incrementCellsNumsAround(const int col, const int row)
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

void Field::checkCellsAround(const int col, const int row)
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

            if (m_openingCells)
                m_cellsToOpen.push(m_cellsData[currColST][currRowST]);

            else
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

int32_t Field::toMapNum(const int32_t col, const int32_t row) const
{
    return col * m_fieldHeightCells + row;
}

std::pair<int32_t, int32_t> Field::fromMapNum(const int32_t mapNum) const
{
    const int32_t col{ mapNum / m_fieldHeightCells };
    const int32_t row{ mapNum % m_fieldHeightCells };

    return std::make_pair(col, row);
}

QByteArray Field::toByteArray() const
{
    QByteArray dataArray{};

    QDataStream dataStream(&dataArray, QIODevice::WriteOnly);
    dataStream.setByteOrder(QDataStream::BigEndian);
    dataStream.setVersion(QDataStream::Qt_6_10);

    dataStream << m_fieldWidthCells << m_fieldHeightCells;

    const int32_t minesMapLength{ static_cast<int32_t>(m_minesMap.size()) * 4 };
    dataStream << minesMapLength;
    for (const int32_t num : m_minesMap)
        dataStream << num;

    const int32_t checkedMapLength{ static_cast<int32_t>(m_checkedMap.size()) * 4 };
    dataStream << checkedMapLength;
    for (const int32_t num : m_checkedMap)
        dataStream << num;

    const int32_t markedMapLength{ static_cast<int32_t>(m_markedMap.size()) * 4};
    dataStream << markedMapLength;
    for (const int32_t num : m_markedMap)
        dataStream << num;

    return dataArray;
}
void Field::openCellsArea(const BasicCell* startCell)
{
    m_openingCells = true;

    checkCellsAround(startCell->getCol(), startCell->getRow());

    while (!m_cellsToOpen.empty())
    {
        BasicCell* cell{ m_cellsToOpen.front() };
        m_cellsToOpen.pop();

        cell->check();
    }

    m_openingCells = false;
}