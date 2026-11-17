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

    m_connection->sendWaitField(m_playerNum);
}

void Client::onDisconnected()
{
    std::cerr << "Client: disconnected" << std::endl;
}

void Client::receivedLeftClickSlot(const std::int8_t playerNum,
    const std::int32_t col, const std::int32_t row)
{
    std::cerr << "Player " << playerNum << " click LEFT button on("
        << col << ", " << row << ')' << std::endl;

    emit fromOnlineLeftClickSignal(playerNum, col, row);
}

void Client::receivedRightClickSlot(const std::int8_t playerNum,
    const std::int32_t col, const std::int32_t row)
{
    std::cerr << "Player " << playerNum << " click RIGHT button on cell ("
        << col << ", " << row << ')' << std::endl;

    emit fromOnlineRightClickSignal(playerNum, col, row);
}

void Client::receivedFieldSlot(const QByteArray& fieldData)
{
    std::cerr << "Income field" << std::endl;

    emit fromOnlineFieldSignal(fieldData);
}

void Client::receivedTimerSlot(const std::int64_t accumulatedTime)
{
    std::cerr << "Income timer" << std::endl;

    emit fromOnlineTimerSignal(accumulatedTime);
}

void Client::receivedRestartGameSlot(const std::int8_t playerNum)
{
    std::cerr << "Player " << playerNum << " restart the game" << std::endl;

    emit fromOnlineRestartGameSignal(playerNum);
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
    connect(m_connection, &ConnectionWorker::receivedFieldSignal,
            this, &Client::receivedFieldSlot);
    connect(m_connection, &ConnectionWorker::receivedTimerSignal,
            this, &Client::receivedTimerSlot);
    connect(m_connection, &ConnectionWorker::receivedRestartGameSignal,
            this, &Client::receivedRestartGameSlot);
}

void Client::setPlayerNum(const std::int8_t playerNum)
{
    m_playerNum = playerNum;
}

void Client::connectToHost(const QHostAddress& address, quint16 port)
{
    m_connection->connectToHost(address, port);
}

void Client::restartGame()
{
    m_connection->sendRestartGame(m_playerNum);
}

void Client::waitField()
{
    m_connection->sendWaitField(m_playerNum);
}