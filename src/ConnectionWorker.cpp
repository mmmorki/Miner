#include "Enums.h"
#include "Constants.h"

#include "ConnectionWorker.h"

#include <iostream>

void ConnectionWorker::onReadyRead()
{
    m_data.append(readAll());
    checkDataReady();
}

ConnectionWorker::ConnectionWorker(QObject* parent)
    : QTcpSocket{ parent }
{
    connect(this, &ConnectionWorker::readyRead,
            this, &ConnectionWorker::onReadyRead);
}

void ConnectionWorker::sendLeftClick(const std::int8_t playerNum,
    const std::int32_t col, const std::int32_t row)
{
    QByteArray message{};

    QDataStream dataStream{ &message, QIODevice::WriteOnly };
    dataStream.setByteOrder(QDataStream::BigEndian);

    dataStream << Constants::LEFT_CLICK_MESSAGE_LENGTH;
    dataStream << NetworkEnums::LeftClick;
    dataStream << playerNum;
    dataStream << col << row;

    write(message);
}

void ConnectionWorker::sendRightClick(const std::int8_t playerNum,
    const std::int32_t col, const std::int32_t row)
{
    QByteArray message{};

    QDataStream dataStream{ &message, QIODevice::WriteOnly };
    dataStream.setByteOrder(QDataStream::BigEndian);

    dataStream << Constants::RIGHT_CLICK_MESSAGE_LENGTH;
    dataStream << NetworkEnums::RightClick;
    dataStream << playerNum;
    dataStream << col << row;

    write(message);
}

void ConnectionWorker::checkDataReady()
{
    while (true)
    {
        if (m_data.size() < 4)
            return;

        QDataStream dataStream{ &m_data, QIODevice::ReadOnly };
        dataStream.setByteOrder(QDataStream::BigEndian);

        std::int32_t length{};
        dataStream >> length;

        if (m_data.size() >= length)
        {
            QByteArray message{ m_data.sliced(0, length) };
            m_data.remove(0, length);
            processMessage(message);
        }

        else
            return;
    }

}

void ConnectionWorker::processMessage(QByteArray& message)
{
    QDataStream dataStream{ &message, QIODevice::ReadOnly };
    dataStream.setByteOrder(QDataStream::BigEndian);

    std::int32_t length{};
    NetworkEnums::MessageType messageType{};
    std::int8_t playerNum{};

    dataStream >> length;
    dataStream >> messageType;
    dataStream >> playerNum;

    std::cerr << length << ' ' << messageType << ' ' << playerNum << ' ';

    switch (messageType)
    {
        case NetworkEnums::LeftClick:
        {
            std::int32_t col{};
            std::int32_t row{};

            dataStream >> col;
            dataStream >> row;

            std::cerr << col << ' ' << row << std::endl;

            emit receivedLeftClickSignal(playerNum, col, row);
            break;
        }

        case NetworkEnums::RightClick:
        {
            std::int32_t col{};
            std::int32_t row{};

            dataStream >> col;
            dataStream >> row;

            std::cerr << col << ' ' << row << std::endl;

            emit receivedRightClickSignal(playerNum, col, row);
            break;
        }

        default:
            return;
    }
}