#ifndef SERVER_H
#define SERVER_H

#include "Enums.h"

#include <QTcpServer>
#include <QTcpSocket>

using namespace NetworkEnums;

class Server final : public QTcpServer
{
    Q_OBJECT

public: signals:
    void fromOnlineLeftClickSignal(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);
    void fromOnlineRightClickSignal(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);

public slots:
    void toOnlineLeftClickSlot(std::int32_t col, std::int32_t row);
    void toOnlineRightClickSlot(std::int32_t col, std::int32_t row);

private slots:
    void onReadyRead();
    void onDisconnected();

public:
    explicit Server(QObject* parent = nullptr);
    ~Server() override = default;

protected:
    void incomingConnection(qintptr socketDescriptor) override;

    void checkData();
    void processMessage(QByteArray& messageArray);

private:
    std::int8_t m_playerNum{ 0 };
    std::vector<QTcpSocket*> m_clients{};
    QByteArray m_data{};
};

#endif //SERVER_H