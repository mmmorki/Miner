#include "Server.h"

#include <algorithm>
#include <iostream>

void Server::onReadyRead()
{
    auto* client{ dynamic_cast<QTcpSocket*>(sender()) };
    if (!client)
        return;

    m_data.append(client->readAll());
    checkData();
}

void Server::onDisconnected()
{
    auto* client{ dynamic_cast<QTcpSocket*>(sender()) };
    if (!client)
        return;

    const auto clientIter{ std::find(m_clients.begin(), m_clients.end(), client) };
    m_clients.erase(clientIter, clientIter + 1);
    client->deleteLater();
}

void Server::toOnlineLeftClickSlot(const std::int32_t col,
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

    for (auto* client : m_clients)
        client->write(message);
}

void Server::toOnlineRightClickSlot(const std::int32_t col,
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

    for (auto* client : m_clients)
        client->write(message);
}

Server::Server(QObject* parent)
    : QTcpServer{ parent }
{
}

void Server::incomingConnection(const qintptr socketDescriptor)
{
    auto* client{ new QTcpSocket{ this } };
    m_clients.push_back(client);

    client->setSocketDescriptor(socketDescriptor);

    connect(client, &QTcpSocket::readyRead,
            this, &Server::onReadyRead);
    connect(client, &QTcpSocket::disconnected,
            this, &Server::onDisconnected);

    std::cerr << "new connection" << std::endl;
}

void Server::checkData()
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

void Server::processMessage(QByteArray& messageArray)
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