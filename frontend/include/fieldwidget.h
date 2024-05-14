#pragma once

#include <QLabel>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "clock.h"
#include "game.h"

const size_t CELL_SIZE = 50;

class FieldWidget : public QTableWidget {
    Q_OBJECT
public:
    explicit FieldWidget(Game *game, size_t player, bool hide, QLabel* status, Clock *clock, QWidget *parent = nullptr);
    virtual ~FieldWidget() {}

    void UpdateGame(Game *game);
    void Update();

    FieldWidget* secondField = nullptr;

signals:
    void difficultyChangeRequested();

private slots:
    void CellClicked(size_t x, size_t y);

private:
    Game* game_ = nullptr;
    size_t player_;
    bool hide_;
    QLabel* status_;
    Clock* clock_;
};
