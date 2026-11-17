#ifndef CONNECTIONWORKER_H
#define CONNECTIONWORKER_H

#include <vector>

#include <QTcpSocket>

/* Схемы сообщений:
 * LeftClick:   ****(length) *(type) *(playerNum) ****(col) ****(row)
 * RightClick:  ****(length) *(type) *(playerNum) ****(col) ****(row)
 * ShareField:  ****(length) *(type) ****(width) ****(height)
 *              ****(minesMapLength) ****...(mines)
 *              ****(checkedMapLength) ****...(checked)
 *              ****(markedMapLength) ****...(marked)
 * ShareTimer:  ****(length) *(type) ********(time)
 * WaitField:   ****(length) *(type) *(playerNum)
 * RestartGame: ****(length) *(type) *(playerNum)
 */

class ConnectionWorker final : public QTcpSocket
{
    Q_OBJECT

public: signals:
    void receivedLeftClickSignal(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);
    void receivedRightClickSignal(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);
    void receivedFieldSignal(const QByteArray& fieldData); //Только для клиента
    void receivedTimerSignal(std::int64_t accumulatedTime); //Только для клиента
    void receivedWaitFieldSignal(std::int8_t playerNum); //Только для хоста
    void receivedRestartGameSignal(std::int8_t playerNum);

private slots:
    void onReadyRead();

public:
    explicit ConnectionWorker(QObject* parent = nullptr);
    ~ConnectionWorker() override = default;

    void sendLeftClick(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);
    void sendRightClick(std::int8_t playerNum, std::int32_t col,
        std::int32_t row);
    void sendField(const QByteArray& fieldData);
    void sendTimer(std::int64_t accumulatedTime);
    void sendWaitField(std::int8_t playerNum);
    void sendRestartGame(std::int8_t playerNum);

protected:
    void checkDataReady();
    void processMessage(QByteArray& message);

private:
    QByteArray m_data{};
};

#endif //CONNECTIONWORKER_H