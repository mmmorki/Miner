#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <sstream>
#include <vector>
#include <random>
#include <algorithm>

#include <QString>

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

template<typename T>
bool contains(const std::vector<T>& vector, const T& value)
{
    return std::find(vector.begin(), vector.end(), value) != vector.end();
}

#endif //FUNCTIONS_H