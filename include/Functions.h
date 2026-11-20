#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <sstream>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

#include <QString>
#include <QPixmapCache>

inline QPixmap getCachedPixmap(const QString& fileName)
{
    QPixmap pixmap{};

    if (QPixmapCache::find(fileName, &pixmap))
        return pixmap;

    pixmap.load(fileName);
    QPixmapCache::insert(fileName, pixmap);
    std::cerr << fileName.toStdString() << " loaded" << std::endl;
    return pixmap;
}

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