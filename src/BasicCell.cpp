#include "Constants.h"

#include "BasicCell.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

BasicCell::BasicCell(const int col, const int row, QGraphicsItem* parent)
    : QGraphicsObject { parent }
    , m_col           { col }
    , m_row           { row }
{
    setAcceptedMouseButtons(Qt::MouseButtons{ Qt::LeftButton, Qt::RightButton });
}

int BasicCell::getCol() const { return m_col; }
int BasicCell::getRow() const { return m_row; }
bool BasicCell::isChecked() const { return m_checked; }
bool BasicCell::isMarked() const { return m_marked; }

void BasicCell::check()
{
    if (!isMarked() && !isChecked())
    {
        m_checked = true;
        update();
        emit cellCheckedSignal();
    }
}

void BasicCell::mark()
{
    if (!isChecked())
    {
        m_marked = true;
        update();
    }
}

void BasicCell::unmark()
{
    if (!isChecked())
    {
        m_marked = false;
        update();
    }
}

QRectF BasicCell::boundingRect() const
{
    return QRectF{ 0.0, 0.0, Constants::CELL_WIDTH_PX,
                             Constants::CELL_HEIGHT_PX };
}

void BasicCell::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        emit leftClickedCellSignal();

    else if (event->button() == Qt::RightButton)
    {
        if (isMarked())
            unmark();

        else
            mark();
    }

    event->accept();
}