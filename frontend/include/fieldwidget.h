#pragma once

#include <QLabel>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "game.h"

const size_t CELL_SIZE = 50;

class FieldWidget : public QTableWidget {
    Q_OBJECT
public:
    explicit FieldWidget(Game *game, size_t player, bool hide, QLabel* status, QWidget *parent = nullptr);
    virtual ~FieldWidget() {}

    void UpdateGame(Game *game);
    void Update();

    FieldWidget* secondField = nullptr;

private slots:
    void CellClicked(size_t x, size_t y);

private:
    Game* game_ = nullptr;
    size_t player_;
    bool hide_;
    QLabel* status_;
};
