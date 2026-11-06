#ifndef CLIENT_H
#define CLIENT_H

#include "qhostaddress.h"
#include <QObject>

class ConnectionWorker;

class Client final : public QObject
{
    Q_OBJECT

public: signals:
    /* Сигналы для отправки полученных от сокета команд на поле */
    void fromOnlineLeftClickSignal(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);
    void fromOnlineRightClickSignal(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);

public slots:
    /* Слоты для получения сетевых команд от поля и последующей отправки в
     * сокет */
    void toOnlineLeftClickSlot(std::int32_t col, std::int32_t row);
    void toOnlineRightClickSlot(std::int32_t col, std::int32_t row);

private slots:
    void onConnected();
    void onDisconnected();

    /* Слоты для получения сетевых команд от сокета и последующей отправки
     * на поле при помощи "fromOnline" сигналов */
    void receivedLeftClickSlot(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);
    void receivedRightClickSlot(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);

public:
    explicit Client(QObject* parent = nullptr);
    ~Client() override = default;

    void setPlayerNum(std::int8_t playerNum);
    void connectToHost(const QHostAddress& address, quint16 port);

private:
    ConnectionWorker* m_connection{ nullptr };
    std::int8_t m_playerNum{ 0 };
};

#endif //CLIENT_H