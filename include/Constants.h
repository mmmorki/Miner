#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

namespace Constants
{
    static int DEFAULT_FIELD_WIDTH_CELLS{ 10 };
    static QString DEFAULT_FIELD_WIDTH_CELLS_STR{
        QString::number(DEFAULT_FIELD_WIDTH_CELLS) };

    static int DEFAULT_FIELD_HEIGHT_CELLS{ 10 };
    static QString DEFAULT_FIELD_HEIGHT_CELLS_STR{
        QString::number(DEFAULT_FIELD_HEIGHT_CELLS) };

    static int DEFAULT_MINES_COUNT{ 10 };
    static QString DEFAULT_MINES_COUNT_STR{
        QString::number(DEFAULT_MINES_COUNT) };

    static double CELL_WIDTH_PX{ 40.0 };
    static double CELL_HEIGHT_PX{ 40.0 };

    static char OBJECT_IN_DEV_STRING[] { "Объект в разработке" };
}

#endif //CONSTANTS_H