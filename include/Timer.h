#ifndef TIMER_H
#define TIMER_H

#include <QLabel>
#include <QElapsedTimer>

class QTimer;

class Timer final : public QLabel
{
public:
    explicit Timer(QWidget* parent = nullptr);
    ~Timer() override = default;

    void start();
    void stop();

    [[nodiscard]] std::int64_t getAccumulatedTime() const;
    void setAccumulatedTime(std::int64_t accumulatedTime);

protected:
    void updateLabel();

private:
    QElapsedTimer m_elapsedTimer;
    QTimer* m_updateTimer{ nullptr };
    std::int64_t m_accumulatedTime{ 0 };
};

#endif //TIMER_H