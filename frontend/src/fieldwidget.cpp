#include "fieldwidget.h"

#include <iostream>

/**
 * \brief Конструктор сетки поля
*/
FieldWidget::FieldWidget(Game *game, size_t player, bool hide, QLabel* status, Clock *clock, QWidget *parent)
    : QTableWidget(parent), game_(game), player_(player), hide_(hide), status_(status), clock_(clock) {
    setRowCount(10);
    setColumnCount(10);

    for (int c = 0; c < horizontalHeader()->count(); ++c) {
        horizontalHeader()->setSectionResizeMode(c, QHeaderView::Fixed);
        horizontalHeader()->setDefaultSectionSize(CELL_SIZE);
    }

    for (int r = 0; r < verticalHeader()->count(); ++r) {
        verticalHeader()->setSectionResizeMode(r, QHeaderView::Fixed);
        verticalHeader()->setDefaultSectionSize(CELL_SIZE);
    }

    verticalHeader()->setVisible(false);
    horizontalHeader()->setVisible(false);

    Update();

    connect(this, &QTableWidget::cellClicked, this, &FieldWidget::CellClicked);
}

/**
 * \brief Функция вызова апдейта поля
*/
void FieldWidget::UpdateGame(Game* game) {
    game_ = game;
    Update();
}

/**
 * \brief Обновление сетки поля
 * \details Заново отрисовывает поле, показывая все последние обновления на нем
*/
void FieldWidget::Update() {
    auto field = game_->GetField(player_);
    if (field == nullptr) return;
    auto c = field->Prepare(hide_);
    
    for (size_t i = 0; i < c.size(); ++i) {
        for (size_t j = 0; j < c.size(); ++j) {
            QTableWidgetItem* tableItem = item(i, j);
            bool newItem = false;
            if (!tableItem) {
                tableItem = new QTableWidgetItem;
                newItem = true;
            }
            if (c[i][j] == 'c') {
                tableItem->setBackground(QBrush(Qt::red));
            } else if (c[i][j] == '#') {
                tableItem->setBackground(QBrush(Qt::white));
            } else if (c[i][j] == 'a') {
                tableItem->setBackground(QBrush(QColorConstants::Svg::purple));
            } else if (c[i][j] == 'b') {
                tableItem->setBackground(QBrush(Qt::black));
            }
            if (newItem) setItem(i, j, tableItem);
        }
    }
}

/**
 * \brief Функция нажатия на ячейку
 * \details Дает результат нажатия на ячейку. Производит ход игрока, ход компьютера и делает проверку на наличие победителя в игре
*/
void FieldWidget::CellClicked(size_t x, size_t y) {
    if (player_ == 0) return;
    clock_->Reset();
    game_->MakeMove1(x, y);
    clearSelection();
    Update();
    secondField->Update();
    if (game_->Win(0)) {
        status_->setText("You win");
    }
    if (game_->Win(1)) {
        status_->setText("You lose");
    }
    emit difficultyChangeRequested();
}
