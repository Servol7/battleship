#include "ship.h"

/**
 * \brief Конструктор корабля
 * \details Имеет размер корабля, координаты его левой верхней ячейки, ориентацию на поле
*/
Ship::Ship(size_t size, size_t x, size_t y, bool ornt) : size_(size), crd_({x, y}), ornt_(ornt) {}
/**
 * \brief Еще один конструктор корабля
 * \details Имеет размер корабля, координаты его левой верхней ячейки, ориентацию на поле
*/
Ship::Ship(size_t size, const std::pair<size_t, size_t>& crd, bool ornt) : size_(size), crd_(crd), ornt_(ornt) {}

/**
 * \brief Функция возвращает размер корабля
*/
size_t Ship::GetSize() const {
    return size_;
}

/**
 * \brief Функция возвращает координату X левой верхней ячейки
*/
size_t Ship::GetX() const {
    return crd_.first;
}

/**
 * \brief Функция возвращает координату Y левой верхней ячейки
*/
size_t Ship::GetY() const {
    return crd_.second;
}

/**
 * \brief Функция возвращает обе координаты левой верхней ячейки
*/
std::pair<size_t, size_t> Ship::GetCrd() const {
    return crd_;
}

/**
 * \brief Функция возвращает ориентацию корабля
*/
bool Ship::GetOrientation() const {
    return ornt_;
}

/**
 * \brief Функция считает количество пробитий
*/
void Ship::AddHit() {
    ++hits_;
}

/**
 * \brief Функция оценивает мертвость корабля
*/
bool Ship::CheckDead() const {
    return hits_ == size_;
}
