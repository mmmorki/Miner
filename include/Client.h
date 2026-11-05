#ifndef CLIENT_H
#define CLIENT_H

#include "Enums.h"

#include <QTcpSocket>

using namespace NetworkEnums;

class Client final : public QTcpSocket
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

public:
    explicit Client(QObject* parent = nullptr);
    ~Client() override = default;

    void setPlayerNum(std::int8_t playerNum);

protected:
    void checkData();
    void processMessage(QByteArray& messageArray);

private:
    std::int8_t m_playerNum{};
    QByteArray m_data{};
};

#endif //CLIENT_H