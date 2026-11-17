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
    dataStream.setVersion(QDataStream::Qt_6_10);

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
    dataStream.setVersion(QDataStream::Qt_6_10);

    dataStream << Constants::RIGHT_CLICK_MESSAGE_LENGTH;
    dataStream << NetworkEnums::RightClick;
    dataStream << playerNum;
    dataStream << col << row;

    write(message);
}

void ConnectionWorker::sendField(const QByteArray& fieldData)
{
    QByteArray message{};

    QDataStream dataStream{ &message, QIODevice::WriteOnly };
    dataStream.setByteOrder(QDataStream::BigEndian);
    dataStream.setVersion(QDataStream::Qt_6_10);

    const std::int32_t length{ 5 + static_cast<std::int32_t>(fieldData.size()) };
    dataStream << length;
    dataStream << NetworkEnums::ShareField;
    dataStream.writeRawData(fieldData.constData(), fieldData.size());

    write(message);
}

void ConnectionWorker::sendWaitField(const std::int8_t playerNum)
{
    QByteArray message{};

    QDataStream dataStream{ &message, QIODevice::WriteOnly };
    dataStream.setByteOrder(QDataStream::BigEndian);
    dataStream.setVersion(QDataStream::Qt_6_10);

    dataStream << Constants::WAIT_FIELD_MESSAGE_LENGTH;
    dataStream << NetworkEnums::WaitField;
    dataStream << playerNum;

    write(message);
}

void ConnectionWorker::sendRestartGame(const std::int8_t playerNum)
{
    QByteArray message{};

    QDataStream dataStream{ &message, QIODevice::WriteOnly };
    dataStream.setByteOrder(QDataStream::BigEndian);
    dataStream.setVersion(QDataStream::Qt_6_10);

    dataStream << Constants::RESTART_GAME_MESSAGE_LENGTH;
    dataStream << NetworkEnums::RestartGame;
    dataStream << playerNum;

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
        dataStream.setVersion(QDataStream::Qt_6_10);

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
    dataStream.setVersion(QDataStream::Qt_6_10);

    std::int32_t length{};
    NetworkEnums::MessageType messageType{};


    dataStream >> length;
    dataStream >> messageType;

    std::cerr << length << ' ' << messageType << ' ';

    switch (messageType)
    {
        case NetworkEnums::LeftClick:
        {
            std::int8_t playerNum{};
            std::int32_t col{};
            std::int32_t row{};

            dataStream >> playerNum;
            dataStream >> col >> row;

            std::cerr << "Left click" << std::endl;
            std::cerr << playerNum << ' ' << col << ' ' << row << std::endl;

            emit receivedLeftClickSignal(playerNum, col, row);
            break;
        }

        case NetworkEnums::RightClick:
        {
            std::int8_t playerNum{};
            std::int32_t col{};
            std::int32_t row{};

            dataStream >> playerNum;
            dataStream >> col >> row;

            std::cerr << "Right click" << std::endl;
            std::cerr << playerNum << ' ' << col << ' ' << row << std::endl;

            emit receivedRightClickSignal(playerNum, col, row);
            break;
        }

        case NetworkEnums::ShareField:
        {
            std::cerr << "Share field" << std::endl;
            emit receivedFieldSignal(message.sliced(5));
            break;
        }

        case NetworkEnums::WaitField:
        {
            std::int8_t playerNum{};

            dataStream >> playerNum;

            emit receivedWaitFieldSignal(playerNum);
            break;
        }

        case NetworkEnums::RestartGame:
        {
            std::int8_t playerNum{};

            dataStream >> playerNum;

            emit receivedRestartGameSignal(playerNum);
            break;
        }

        default:;
    }
}