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
}

#endif //ENUMS_H