#include "mainwindow.h"

#include <iostream>

/**
 * \brief Конструктор окна игры
 * \details Создает два игровых поля. Создает все кнопки и панели отображения информации
*/
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    game_ = new Game;

    status = new QLabel("Status: running");
    clock_ = new Clock(this);

    field1_ = new FieldWidget(game_, 0, false, status, clock_);
    field2_ = new FieldWidget(game_, 1, true, status, clock_);
    field1_->secondField = field2_;
    field2_->secondField = field1_;

    restartButton_ = new QPushButton("Restart");
    connect(restartButton_, &QPushButton::clicked, this, &MainWindow::RestartClicked);

    QPushButton *saveButton = new QPushButton("Save Game", this);
    QPushButton *loadButton = new QPushButton("Load Game", this);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::SaveClicked);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::LoadClicked);

    buttonLayout_ = new QVBoxLayout;
    buttonLayout_->addWidget(clock_);
    SetupDifficultyComboBox();
    buttonLayout_->addStretch();
    buttonLayout_->addWidget(status);
    buttonLayout_->addWidget(restartButton_);
    buttonLayout_->addWidget(saveButton);
    buttonLayout_->addWidget(loadButton);
    buttonLayout_->addStretch();

    RestartClicked(); 
    connect(field1_, &FieldWidget::difficultyChangeRequested, this, &MainWindow::LockDifficulty);
    connect(field2_, &FieldWidget::difficultyChangeRequested, this, &MainWindow::LockDifficulty);
    UnlockDifficultySelection();

    layout_ = new QHBoxLayout;
    layout_->setContentsMargins(0, 0, 0, 0);
    layout_->setSpacing(0);
    layout_->addWidget(field1_);
    layout_->addLayout(buttonLayout_);
    layout_->addWidget(field2_);

    centralWidget_ = new QWidget(this);
    centralWidget_->setLayout(layout_);

    setCentralWidget(centralWidget_);
    setFixedSize(20 * CELL_SIZE + 150, 10 * CELL_SIZE + 20);
}

/**
 * \brief Деструктор окна игры
*/
MainWindow::~MainWindow() {
    delete game_;
}

/**
 * \brief Функция рестарта игры по клику кнопки
*/
void MainWindow::RestartClicked() {
    delete game_;
    game_ = new Game;
    field1_->UpdateGame(game_);
    field2_->UpdateGame(game_);
    status->setText("Status: running");
    difficultyComboBox_->setCurrentIndex(static_cast<int>(Difficulty::Easy));
    UnlockDifficultySelection();
}

/**
 * \brief Функция сохранения игры по клику кнопки
*/
void MainWindow::SaveClicked() {
    game_->SaveGame("game_save.txt");
    LockDifficultySelection();
}

/**
 * \brief Функция загрузки сохранения по клику кнопки
*/
void MainWindow::LoadClicked() {
    game_->LoadGame("game_save.txt");
    field1_->UpdateGame(game_);
    field2_->UpdateGame(game_);
    UnlockDifficultySelection();
}

/**
 * \brief Настройка выпадающего списка для выбора уровня сложности
*/
void MainWindow::SetupDifficultyComboBox() {
    difficultyComboBox_ = new QComboBox;
    difficultyComboBox_->addItem("Easy", QVariant::fromValue(Difficulty::Easy));
    difficultyComboBox_->addItem("Medium", QVariant::fromValue(Difficulty::Medium));
    difficultyComboBox_->addItem("Hard", QVariant::fromValue(Difficulty::Hard));
    connect(difficultyComboBox_, QOverload<int>::of(&QComboBox::activated), [this](int index) {
        game_->SetDifficulty(static_cast<Difficulty>(difficultyComboBox_->itemData(index).toInt()));
    });

    buttonLayout_->addWidget(difficultyComboBox_);
}

/**
 * \brief Отключение возможности выбора сложности
*/
void MainWindow::LockDifficultySelection() {
    difficultyComboBox_->setEnabled(false);
}

/**
 * \brief Включение возможности выбора сложности
*/
void MainWindow::UnlockDifficultySelection() {
    difficultyComboBox_->setEnabled(true);
}

/**
 * \brief Отключение возможности выбора сложности номер два
*/
void MainWindow::LockDifficulty() {
    difficultyComboBox_->setEnabled(false);
}