#include "game.h"

#include <iostream>
#include <chrono>
#include <random>

/**
 * \brief Конструктор класса Game
 * \details Создает два игровых поля
*/
Game::Game() {
    field_[0] = new Field();
    field_[1] = new Field();
}

/**
 * \brief Деструктор класса Game
*/
Game::~Game() {
    delete field_[0];
    delete field_[1];
}

/**
 * \brief Функция возвращения поля
 * \details Получает параметр i - номер поля и по нему выводит соответсвующее поле
*/
Field* Game::GetField(size_t i) const {
    return field_[i];
}

/**
 * \brief Проверка конца игры
 * \details Проверяет оба поля на наличие живых кораблей
*/
bool Game::EndOfGame() const {
    return field_[0]->CheckDead() || field_[1]->CheckDead();
}

/**
 * \brief Проверка наличия победителя
*/
bool Game::Win(size_t i) const {
    return field_[1 - i]->CheckDead();
}

/**
 * \brief Ход игрока
 * \details Получает координаты ячейки и атакует эту ячейку на поле компьютера. Если произошло попадание, снова запускает ход игрока
*/
void Game::MakeMove1(size_t x, size_t y) {
    if (EndOfGame()) {
        return;
    }
    if (field_[1]->MakeHit(x, y) == 1 && !EndOfGame()) {
        MakeMove2();
    }
}

/**
 * \brief Ход компьютера
 * \details Генерирует координаты еще не атакованной ячейки и атакует ее на поле игрока. Если произошло попадание, снова запускает ход компьютера
*/
void Game::MakeMove2() {
    std::mt19937 rnd(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    size_t x, y;
    int res;
    do {
        x = rnd() % FIELD_SIZE;
        y = rnd() % FIELD_SIZE;
    } while (!Win(1) && !(res = field_[0]->MakeHit(x, y)));
    if (res == 2) {
        MakeMove2();
    }
}
