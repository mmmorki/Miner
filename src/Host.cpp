#include "Host.h"

#include "ConnectionWorker.h"

#include <iostream>

void Host::toOnlineLeftClickSlot(const std::int32_t col, const std::int32_t row)
{
    for (ConnectionWorker* connection : m_connections)
        if (connection->state() == QAbstractSocket::ConnectedState)
            connection->sendLeftClick(m_playerNum, col, row);
}

void Host::toOnlineRightClickSlot(const std::int32_t col, const std::int32_t row)
{
    for (ConnectionWorker* connection : m_connections)
        if (connection->state() == QAbstractSocket::ConnectedState)
            connection->sendRightClick(m_playerNum, col, row);
}

void Host::incomingConnection(const qintptr descriptor)
{
    auto* connection{ new ConnectionWorker{ this } };
    connection->setSocketDescriptor(descriptor);

    connect(connection, &ConnectionWorker::disconnected,
            this, &Host::onDisconnected);

    connect(connection, &ConnectionWorker::receivedLeftClickSignal,
            this, &Host::receivedLeftClickSlot);
    connect(connection, &ConnectionWorker::receivedRightClickSignal,
            this, &Host::receivedRightClickSlot);
    connect(connection, &ConnectionWorker::receivedRestartGameSignal,
            this, &Host::receivedRestartGameSlot);
    connect(connection, &ConnectionWorker::receivedWaitFieldSignal,
            this, &Host::receivedWaitFieldSlot);

    m_connections.push_back(connection);
}

void Host::onDisconnected()
{
    auto* connection{ dynamic_cast<ConnectionWorker*>(sender()) };

    const auto connIter{ std::find(m_connections.begin(), m_connections.end(),
        connection) };
    m_connections.erase(connIter);

    connection->deleteLater();
}

void Host::receivedLeftClickSlot(const std::int8_t playerNum,
    const std::int32_t col, const std::int32_t row)
{
    emit fromOnlineLeftClickSignal(playerNum, col, row);
}

void Host::receivedRightClickSlot(const std::int8_t playerNum,
    const std::int32_t col, const std::int32_t row)
{
    emit fromOnlineRightClickSignal(playerNum, col, row);
}

void Host::receivedRestartGameSlot(const std::int8_t playerNum)
{
    emit fromOnlineRestartGameSignal(playerNum);
}

void Host::receivedWaitFieldSlot(const std::int8_t playerNum)
{
    emit fromOnlineWaitFieldSignal(playerNum);
}

Host::Host(QObject* parent)
    : QTcpServer{ parent }
{
}

void Host::shareField(const QByteArray& fieldData) const
{
    for (ConnectionWorker* connection : m_connections)
        if (connection->state() == QAbstractSocket::ConnectedState)
            connection->sendField(fieldData);
}

void Host::shareTimer(const std::int64_t accumulatedTime) const
{
    for (ConnectionWorker* connection : m_connections)
        if (connection->state() == QAbstractSocket::ConnectedState)
            connection->sendTimer(accumulatedTime);
}

void Host::restartGame() const
{
    for (ConnectionWorker* connection : m_connections)
        if (connection->state() == QAbstractSocket::ConnectedState)
            connection->sendRestartGame(m_playerNum);
}