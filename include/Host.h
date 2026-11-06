#ifndef HOST_H
#define HOST_H

#include <QTcpServer>

class ConnectionWorker;

class Host final : public QTcpServer
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
    void incomingConnection(qintptr descriptor) override;
    void onDisconnected();

    /* Слоты для получения сетевых команд от сокета и последующей отправки
     * на поле при помощи "fromOnline" сигналов */
    void receivedLeftClickSlot(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);
    void receivedRightClickSlot(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);

public:
    explicit Host(QObject* parent = nullptr);
    ~Host() override = default;

private:
    std::vector<ConnectionWorker*> m_connections{};
    std::int8_t playerNum{ 0 };
};

#endif //HOST_H