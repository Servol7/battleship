#include "clock.h"

/**
 * \brief Конструктор класса Clock
 * \details Содержит в себе объекты для отслеживания и отображения времени
*/
Clock::Clock(QWidget *parent) : QLCDNumber(parent) {
    setSegmentStyle(Filled);
    
    elapsedTimer_ = new QElapsedTimer;
    elapsedTimer_->start();

    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &Clock::ShowTime);
    timer_->start(1000);

    ShowTime();
    setFixedSize(150, 80);
}

/**
 * \brief Функция показа времени
 * \details Извлекает количество прошедших секунд с момента запуска, переводит их в минуты и секунды, и отображает в формате MM:SS.
*/
void Clock::ShowTime() {
    qint64 time = elapsedTimer_->elapsed() / 1000;
    qint64 mm = time / 60;
    qint64 ss = time % 60;
    display(QString::number(mm) + ":" + QString::number(ss));
}

/**
 * \brief Функция сброса таймера
*/
void Clock::Reset() {
    timer_->stop();
    timer_->start();
    elapsedTimer_->restart();
    ShowTime();
}
