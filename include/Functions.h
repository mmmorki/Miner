#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QString>
#include <sstream>

inline int toInt(const QString& strValue)
{
    std::stringstream ss;
    int intValue{};

    ss << strValue.toStdString();
    ss >> intValue;

    return intValue;
}

inline int toInt(const std::string_view strValue)
{
    std::stringstream ss;
    int intValue{};

    ss << strValue;
    ss >> intValue;

    return intValue;
}

#endif //FUNCTIONS_H