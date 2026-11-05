#include "Client.h"

#include <iostream>

void Client::onReadyRead()
{
    m_data.append(readAll());
    checkData();
}

void Client::toOnlineLeftClickSlot(const std::int32_t col,
    const std::int32_t row)
{
    QByteArray message{};

    QDataStream stream(&message, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    stream << static_cast<std::int32_t>(14);
    stream << NetworkEnums::LeftClick;
    stream << m_playerNum;
    stream << col;
    stream << row;

    write(message);
}

void Client::toOnlineRightClickSlot(const std::int32_t col,
    const std::int32_t row)
{
    QByteArray message{};

    QDataStream stream(&message, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    stream << static_cast<std::int32_t>(14);
    stream << NetworkEnums::RightClick;
    stream << m_playerNum;
    stream << col;
    stream << row;

    write(message);
}

Client::Client(QObject* parent)
    : QTcpSocket{ parent }
{
    connect(this, &Client::readyRead, this, &Client::onReadyRead);
}

void Client::setPlayerNum(const std::int8_t playerNum) { m_playerNum = playerNum; }

void Client::checkData()
{
    QDataStream stream{ &m_data, QIODevice::ReadWrite };

    if (m_data.size() < 4)
        return;

    std::int32_t length{};
    stream >> length;
    const std::size_t lengthST{ static_cast<std::size_t>(length) };

    if (m_data.size() >= lengthST)
    {
        QByteArray messageArray{ m_data.left(lengthST) };
        m_data.remove(0, lengthST);
        processMessage(messageArray);
    }
}

void Client::processMessage(QByteArray& messageArray)
{
    QDataStream stream{ &messageArray, QIODevice::ReadWrite };

    std::int32_t length{};
    std::int8_t type{};
    std::int8_t playerNum{};

    stream >> length >> type >> playerNum;

    switch (type)
    {
        case LeftClick:
        {
            std::int32_t col{};
            std::int32_t row{};

            stream >> col >> row;

            emit fromOnlineLeftClickSignal(playerNum, col, row);

            break;
        }
        case RightClick:
        {
            std::int32_t col{};
            std::int32_t row{};

            stream >> col >> row;

            emit fromOnlineRightClickSignal(playerNum, col, row);

            break;
        }
    }
}