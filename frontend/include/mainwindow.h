#pragma once

#include "fieldwidget.h"
#include "clock.h"

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QComboBox>

#include "game.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QLabel *status;

private slots:
    void RestartClicked();
    void SaveClicked();
    void LoadClicked();
    void LockDifficulty();

private:
    FieldWidget *field1_;
    FieldWidget *field2_;
    QHBoxLayout *layout_;
    QVBoxLayout *buttonLayout_;
    QPushButton *restartButton_;
    QWidget *centralWidget_;
    QComboBox* difficultyComboBox_;
    Clock* clock_;
    void SetupDifficultyComboBox();
    void LockDifficultySelection();
    void UnlockDifficultySelection();

    Game *game_;
};
