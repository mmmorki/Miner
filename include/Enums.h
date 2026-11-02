#ifndef ENUMS_H
#define ENUMS_H

#include <cstdint>

namespace MainWindowEnums
{
    enum Page : std::int8_t
    {
        Main,
        Settings,
        Game,
        MaxPage,
    };

    enum InfoBarPage : std::int8_t
    {
        Statistics,
        Victory,
        Defeat,
        MaxInfoBarPage,
    };
}

#endif //ENUMS_H