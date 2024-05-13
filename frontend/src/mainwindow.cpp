#include "mainwindow.h"

#include <iostream>

/**
 * \brief Конструктор окна игры
 * \details Создает два игровых поля. Создает кнопку рестарт для регенерации новых полей. Создает окно статуса игры
*/
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    game_ = new Game;

    status = new QLabel("Status: running");

    field1_ = new FieldWidget(game_, 0, false, status);
    field2_ = new FieldWidget(game_, 1, true, status);
    field1_->secondField = field2_;
    field2_->secondField = field1_;

    restartButton_ = new QPushButton("Restart");
    connect(restartButton_, &QPushButton::clicked, this, &MainWindow::RestartClicked);

    buttonLayout_ = new QVBoxLayout;
    buttonLayout_->addStretch();
    buttonLayout_->addWidget(status);
    buttonLayout_->addWidget(restartButton_);
    buttonLayout_->addStretch();

    layout_ = new QHBoxLayout;
    layout_->setContentsMargins(0, 0, 0, 0);
    layout_->setSpacing(0);
    layout_->addWidget(field1_);
    layout_->addLayout(buttonLayout_);
    layout_->addWidget(field2_);

    centralWidget_ = new QWidget(this);
    centralWidget_->setLayout(layout_);

    setCentralWidget(centralWidget_);
    setFixedSize(20 * CELL_SIZE + 100, 10 * CELL_SIZE + 20);
}

/**
 * \brief Деструктор окна игры
*/
MainWindow::~MainWindow() {
    delete game_;
}

/**
 * \brief Функция рестарта игры
*/
void MainWindow::RestartClicked() {
    delete game_;
    game_ = new Game;
    field1_->UpdateGame(game_);
    field2_->UpdateGame(game_);
    status->setText("Status: running");
}
