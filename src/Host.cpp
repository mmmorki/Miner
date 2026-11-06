#include "../include/Host.h"

#include "ConnectionWorker.h"

void Host::toOnlineLeftClickSlot(const std::int32_t col, const std::int32_t row)
{
    for (ConnectionWorker* connection : m_connections)
        if (connection->state() == QAbstractSocket::ConnectedState)
            connection->sendLeftClick(playerNum, col, row);
}

void Host::toOnlineRightClickSlot(const std::int32_t col, const std::int32_t row)
{
    for (ConnectionWorker* connection : m_connections)
        if (connection->state() == QAbstractSocket::ConnectedState)
            connection->sendRightClick(playerNum, col, row);
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

Host::Host(QObject* parent)
    : QTcpServer{ parent }
{
}
