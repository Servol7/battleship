#include "field.h"

#include <chrono>
#include <iostream>
#include <random>

/**
 * \brief Конструктор класса Field
 */
Field::Field(bool generated) : dead_(0) {
    if (generated) {
        GenerateField();
    }
}

/**
 * \brief Функция добавления корабля
 * \param ship данные о корабле
*/
void Field::AddShip(const Ship& ship) {
    ships_.push_back(ship);
}

/**
 * \brief Проверка валидности расстановки кораблей
 * \details Функция проверяет расстановку кораблей на самопересечения, выход кораблей за границы и постановка кораблей рядом друг с другом
*/
bool Field::IsValid() const {
    std::vector<std::vector<size_t>> f(FIELD_SIZE, std::vector<size_t>(FIELD_SIZE, 0));
    size_t cnt = 1;
    for (const auto& ship : ships_) {
        for (size_t i = 0; i < ship.GetSize(); ++i) {
            size_t x = ship.GetX();
            size_t y = ship.GetY();
            if (ship.GetOrientation()) {
                x += i;
            } else {
                y += i;
            }
            if (x >= FIELD_SIZE || y >= FIELD_SIZE) return false;
            if (f[x][y] != 0) return false;
            f[x][y] = cnt; 
        }
        ++cnt;
    }

    auto check = [&f](size_t i1, size_t j1, size_t i2, size_t j2) {
        return f[i1][j1] == f[i2][j2] || f[i1][j1] == 0 || f[i2][j2] == 0;
    };

    for (size_t i = 0; i < FIELD_SIZE; ++i) {
        for (size_t j = 0; j < FIELD_SIZE; ++j) {
            if (i + 1 < FIELD_SIZE) {
                if (!check(i, j, i + 1, j)) return false;
                if (j + 1 < FIELD_SIZE) {
                    if (!check(i, j, i + 1, j + 1)) return false;
                }
                if (j > 0) {
                    if (!check(i, j, i + 1, j - 1)) return false;
                }
            }
            if (j + 1 < FIELD_SIZE) {
                if (!check(i, j, i, j + 1)) return false;
            }
        }
    }

    return true;
}

/**
 * \brief Вывести поле в консоль
 * \details Функция нужна была для отладки приложения до создания фронтенда
*/
void Field::PrintToCmd() const {
    std::vector<std::vector<size_t>> f(FIELD_SIZE, std::vector<size_t>(FIELD_SIZE, 0));
    size_t cnt = 1;
    for (const auto& ship : ships_) {
        for (size_t i = 0; i < ship.GetSize(); ++i) {
            size_t x = ship.GetX();
            size_t y = ship.GetY();
            if (ship.GetOrientation()) {
                x += i;
            } else {
                y += i;
            }
            f[x][y] = cnt; 
        }
        ++cnt;
    }

    for (size_t i = 0; i < FIELD_SIZE; ++i) {
        for (size_t j = 0; j < FIELD_SIZE; ++j) {
            std::cout << (f[i][j] == 0 ? "#" : std::to_string(f[i][j] - 1));
        }
        std::cout << "\n";
    }
}

/**
 * \brief Создание поля
 * \details Генерирует поля, пока не будет достигнута валидная расстановка
*/
void Field::GenerateField() {
    std::mt19937 rnd(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    do {
        ships_.clear();
        for (size_t size = 1; size <= 4; ++size) {
            for (size_t i = 0; i < 5 - size; ++i) {
                size_t x = rnd() % FIELD_SIZE;
                size_t y = rnd() % FIELD_SIZE;
                bool ornt = rnd() % 2;
                AddShip({size, x, y, ornt});
            }
        }
    } while (!IsValid());
}

/**
 * \brief Проверка завершения игры
 * \details Смотрит количество мертвых кораблей на поле
*/
bool Field::CheckDead() const {
    return ships_.size() == dead_;
}

/**
 * \brief Проверка клетки на промах
*/
bool Field::CheckMiss(int x, int y) const {
    if (0 > x || x >= FIELD_SIZE || 0 > y || y >= FIELD_SIZE) return 0;
    if (hit_.count(std::make_pair(x, y)) && !recentHits_.count(std::make_pair(x, y))) return 0;
    return 1;
}

/**
 * \brief Проверка клетки на наличие выстрела
*/
bool Field::CheckCell(int x, int y) const {
    if (0 > x || x >= FIELD_SIZE || 0 > y || y >= FIELD_SIZE) return 0;
    if (hit_.count(std::make_pair(x, y))) return 0;
    return 1;
}

/**
 * \brief Функция атаки ячейки
 * \details Атакует ячейку с выбранными координатами. Если в этот момент был уничитожен корабль, то обстреливает территорию вокруг него. Выводит значение о промахе или попадании в корабль
*/
int Field::MakeHit(int x, int y) {
    if (0 > x || x >= FIELD_SIZE || 0 > y || y >= FIELD_SIZE) return 0;
    if (hit_.count(std::make_pair(x, y))) return 0;
    hit_.insert({x, y});
    for (auto& ship : ships_) {
        size_t curX = ship.GetX();
        size_t curY = ship.GetY();
        bool hit = false;

        if (ship.GetOrientation()) {
            if (curY == y && curX <= x && x < curX + ship.GetSize()) {
                ship.AddHit();
                hit = true;
            }
        } else {
            if (curX == x && curY <= y && y < curY + ship.GetSize()) {
                ship.AddHit();
                hit = true;
            }
        }

        if (hit){
            recentHits_.insert({x, y});
            if (ship.CheckDead()) {
                ++dead_;
                for (size_t i = 0; i < ship.GetSize(); ++i) {
                    size_t hitX = ship.GetX() + (ship.GetOrientation() ? i : 0);
                    size_t hitY = ship.GetY() + (ship.GetOrientation() ? 0 : i);
                    recentHits_.erase({hitX, hitY});
                }
                if (ship.GetOrientation()) {
                    for (int i = (int) ship.GetX() - 1; i <= (int) ship.GetX() + (int) ship.GetSize(); ++i) {
                        for (int j = (int) ship.GetY() - 1; j <= (int) ship.GetY() + 1; ++j) {
                            MakeHit(i, j);
                        }
                    }
                } else {
                    for (int i = (int) ship.GetX() - 1; i <= (int) ship.GetX() + 1; ++i) {
                        for (int j = (int) ship.GetY() - 1; j <= (int) ship.GetY() + (int) ship.GetSize(); ++j) {
                            MakeHit(i, j);
                        }
                    }
                }
            }
            return 2;
        }
    }
    return 1;
}

/**
 * \brief Функция установки цветов
 * \details Формирует матрицу цветов для закрашивания ячеек. Красный - ячейка с кораблем без выстрела. Белый - пустая ячейка без выстрела. Фиолетовый - пустая ячейка с выстрелом. Черный - пробитая ячейка с кораблем
*/
std::vector<std::string> Field::Prepare(bool hide) const {
    std::vector<std::string> f(FIELD_SIZE, std::string(10, '#'));
    for (auto [x, y] : hit_) {
        f[x][y] = 'a';
    }
    for (const auto& ship : ships_) {
        for (size_t i = 0; i < ship.GetSize(); ++i) {
            size_t x = ship.GetX();
            size_t y = ship.GetY();
            if (ship.GetOrientation()) {
                x += i;
            } else {
                y += i;
            }

            if (f[x][y] == 'a') {
                f[x][y] = 'b';
            } else if (!hide) {
                f[x][y] = 'c';
            }
        }
    }
    return f;
}

/**
 * \brief Функция сохранения поля
 * \details Выводит всю информацию о кораблях и выстрелах
*/
void Field::Save(std::ostream& out) const {
    out << ships_.size() << " " << hit_.size() << " " << dead_ << "\n";
    for (const auto& ship : ships_) {
        out << ship.GetSize() << " " << ship.GetX() << " " << ship.GetY() << " " << ship.GetOrientation() << " " << ship.GetHits() << "\n";
    }
    for (const auto& hit : hit_) {
        out << hit.first << " " << hit.second << "\n";
    }
}

/**
 * \brief Функция загрузки сохранения поля
 * \details Считывает всю информацию о кораблях и выстрелах
*/
void Field::Load(std::istream& in) {
    size_t numShips, numHits;
    in >> numShips >> numHits >> dead_;
    ships_.clear();
    hit_.clear();

    for (size_t i = 0; i < numShips; ++i) {
        size_t size, x, y, hits;
        bool orientation;
        in >> size >> x >> y >> orientation >> hits;
        Ship ship(size, x, y, orientation);
        ship.SetHits(hits);  // Нужно добавить метод SetHits в класс Ship
        ships_.push_back(ship);
    }

    for (size_t i = 0; i < numHits; ++i) {
        size_t x, y;
        in >> x >> y;
        hit_.insert({x, y});
    }
}
