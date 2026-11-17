#include "Timer.h"

#include <QTimer>

Timer::Timer(QWidget* parent)
    : QLabel{ parent }
    , m_updateTimer{ new QTimer{ this } }
{
    m_elapsedTimer.start();
    m_updateTimer->setInterval(1000);

    connect(m_updateTimer, &QTimer::timeout, this, &Timer::updateLabel);
}

void Timer::start()
{
    setText("0:00");

    m_elapsedTimer.restart();
    m_accumulatedTime = 0;
    m_updateTimer->start();
}

void Timer::stop()
{
    updateLabel();
    m_updateTimer->stop();
}

std::int64_t Timer::getAccumulatedTime() const
{
    return m_accumulatedTime;
}

void Timer::setAccumulatedTime(const std::int64_t accumulatedTime)
{
    m_elapsedTimer.restart();
    m_accumulatedTime = accumulatedTime;
}

void Timer::updateLabel()
{
    m_accumulatedTime += m_elapsedTimer.restart();

    const std::int64_t seconds{ m_accumulatedTime / 1000 % 60 };
    const std::int64_t minutes{ m_accumulatedTime / 1000 / 60 };

    setText(QString::number(minutes) + ':'
        + (seconds < 10 ? "0" : "") + QString::number(seconds));
}