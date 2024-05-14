#pragma once

#include <QLCDNumber>
#include <QElapsedTimer>
#include <QTimer>
#include <QWidget>

class Clock : public QLCDNumber {
    Q_OBJECT
public:
    Clock(QWidget *parent = nullptr);
    void Reset();

private slots:
    void ShowTime();

private:
    QElapsedTimer *elapsedTimer_;
    QTimer *timer_;
};
