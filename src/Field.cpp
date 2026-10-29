#include "Enums.h"
#include "Constants.h"

#include "Field.h"

Field::Field(QObject* parent)
    : QGraphicsScene        { parent }
    , m_fieldWidthCells     { Constants::DEFAULT_FIELD_WIDTH_CELLS }
    , m_fieldHeightCells    { Constants::DEFAULT_FIELD_HEIGHT_CELLS }
    , m_minesCount          { Constants::DEFAULT_MINES_COUNT }
{
    const qreal fieldWidthPx{ m_fieldWidthCells * Constants::CELL_WIDTH_PX };
    const qreal fieldHeightPx{ m_fieldHeightCells * Constants::CELL_HEIGHT_PX };
    setSceneRect(0, 0, fieldWidthPx, fieldHeightPx);
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
}