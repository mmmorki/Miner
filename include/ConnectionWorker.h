#ifndef CONNECTIONWORKER_H
#define CONNECTIONWORKER_H

#include <QTcpSocket>
/* Схемы сообщений:
 * LeftClick:   ****(length) *(type) *(playerNum) ****(col) ****(row)
 * RightClick:  ****(length) *(type) *(playerNum) ****(col) ****(row)
 *
 *
 *
 *
 */

class ConnectionWorker final : public QTcpSocket
{
    Q_OBJECT

public: signals:
    void receivedLeftClickSignal(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);
    void receivedRightClickSignal(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);

private slots:
    void onReadyRead();

public:
    explicit ConnectionWorker(QObject* parent = nullptr);
    ~ConnectionWorker() override = default;

    void sendLeftClick(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);
    void sendRightClick(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);

protected:
    void checkDataReady();
    void processMessage(QByteArray& message);

private:
    QByteArray m_data{};
};

#endif //CONNECTIONWORKER_H