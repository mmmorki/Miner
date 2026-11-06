#include "Client.h"

#include "ConnectionWorker.h"

#include <iostream>

void Client::toOnlineLeftClickSlot(const std::int32_t col,
    const std::int32_t row)
{
    if (m_connection->state() == QAbstractSocket::ConnectedState)
        m_connection->sendLeftClick(m_playerNum, col, row);
}

void Client::toOnlineRightClickSlot(const std::int32_t col,
    const std::int32_t row)
{
    if (m_connection->state() == QAbstractSocket::ConnectedState)
        m_connection->sendRightClick(m_playerNum, col, row);
}

void Client::onConnected()
{
    std::cerr << "Client: connected to host" << std::endl;
}

void Client::onDisconnected()
{
    std::cerr << "Client: disconnected" << std::endl;
}

void Client::receivedLeftClickSlot(const std::int8_t playerNum,
    const std::int32_t col, const std::int32_t row)
{
    emit fromOnlineLeftClickSignal(playerNum, col, row);
}

void Client::receivedRightClickSlot(const std::int8_t playerNum,
    const std::int32_t col, const std::int32_t row)
{
    emit fromOnlineRightClickSignal(playerNum, col, row);
}

Client::Client(QObject* parent)
    : QObject{ parent }
    , m_connection{ new ConnectionWorker{ this } }
{
    connect(m_connection, &ConnectionWorker::connected,
            this, &Client::onConnected);
    connect(m_connection, &ConnectionWorker::disconnected,
            this, &Client::onDisconnected);

    connect(m_connection, &ConnectionWorker::receivedLeftClickSignal,
            this, &Client::receivedLeftClickSlot);
    connect(m_connection, &ConnectionWorker::receivedRightClickSignal,
            this, &Client::receivedRightClickSlot);
}

void Client::setPlayerNum(const std::int8_t playerNum)
{
    m_playerNum = playerNum;
}

void Client::connectToHost(const QHostAddress& address, quint16 port)
{
    m_connection->connectToHost(address, port);
}