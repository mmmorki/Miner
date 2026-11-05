#ifndef ENUMS_H
#define ENUMS_H

#include <cstdint>

namespace MainWindowEnums
{
    enum Page : std::int8_t
    {
        Main,
        Settings,
        Connection,
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

    enum Mode : std::int8_t
    {
        None,
        Single,
        Host,
        Client,
        MaxMode,
    };
}

namespace NetworkEnums
{
    enum MessageType : std::int8_t
    {
        LeftClick,
        RightClick,
        ShareField,
    };
}

#endif //ENUMS_H