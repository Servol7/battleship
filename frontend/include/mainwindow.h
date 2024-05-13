#pragma once

#include "fieldwidget.h"

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

#include "game.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QLabel *status;

private slots:
    void RestartClicked();

private:
    FieldWidget *field1_;
    FieldWidget *field2_;
    QHBoxLayout *layout_;
    QVBoxLayout *buttonLayout_;
    QPushButton *restartButton_;
    QWidget *centralWidget_;

    Game *game_;
};
