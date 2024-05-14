#include "game.h"

#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include <algorithm>

Game::Game() {
    field_[0] = new Field();
    field_[1] = new Field();
    probabilityMap = std::vector<std::vector<int>>(FIELD_SIZE, std::vector<int>(FIELD_SIZE, 0));
}

Game::~Game() {
    delete field_[0];
    delete field_[1];
}

Field* Game::GetField(size_t i) const {
    return field_[i];
}

bool Game::EndOfGame() const {
    return field_[0]->CheckDead() || field_[1]->CheckDead();
}

bool Game::Win(size_t i) const {
    return field_[1 - i]->CheckDead();
}

void Game::MakeMove1(size_t x, size_t y) {
    if (EndOfGame()) {
        return;
    }
    if (field_[1]->MakeHit(x, y) == 1 && !EndOfGame()) {
        MakeMove2();
    }
}

void Game::SetDifficulty(Difficulty diff) {
    difficulty_ = diff;
}

void Game::MakeMove2() {
    std::mt19937 rnd(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    size_t x, y;
    int res;
    bool found = false;
    switch (difficulty_) {
        case Difficulty::Easy:
            do {
                x = rnd() % FIELD_SIZE;
                y = rnd() % FIELD_SIZE;
            } while (!Win(1) && !(res = field_[0]->MakeHit(x, y)));
            break;
        case Difficulty::Medium:
            // Проверяем наличие недавних успешных попаданий
            for (auto& hit : field_[0]->GetRecentHits()) { // Предположим, что у нас есть функция GetRecentHits, которая возвращает позиции последних успешных попаданий
                std::vector<std::pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
                std::shuffle(directions.begin(), directions.end(), rnd);
                for (auto& dir : directions) {
                    x = hit.first + dir.first;
                    y = hit.second + dir.second;
                    if (res = field_[0]->MakeHit(x, y)) {
                        found = true;
                        break;
                    }
                }
                if (found) break;
            }
            if (!found) {
                do {
                    x = rnd() % FIELD_SIZE;
                    y = rnd() % FIELD_SIZE;
                } while (!Win(1) && !(res = field_[0]->MakeHit(x, y)));
            }
            break;
        case Difficulty::Hard:
            UpdateProbabilityMap();
            size_t bestX = 0, bestY = 0;
            int maxProbability = -1;
            for (size_t x = 0; x < FIELD_SIZE; ++x) {
                for (size_t y = 0; y < FIELD_SIZE; ++y) {
                    if (probabilityMap[x][y] > maxProbability && !field_[0]->AlreadyHit(x, y)) {
                        maxProbability = probabilityMap[x][y];
                        bestX = x;
                        bestY = y;
                    }
                }
            }
            res = field_[0]->MakeHit(bestX, bestY);
            break;
    }
    if (res == 2) {
        MakeMove2();
    }
}

void Game::SaveGame(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    file << move_ << "\n";
    field_[0]->Save(file);
    field_[1]->Save(file);
    file.close();
}

void Game::LoadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    file >> move_;
    field_[0]->Load(file);
    field_[1]->Load(file);
    file.close();
}

void Game::UpdateProbabilityMap() {
    // Очистить текущую карту вероятностей
    for (size_t x = 0; x < FIELD_SIZE; ++x) {
        for (size_t y = 0; y < FIELD_SIZE; ++y) {
            probabilityMap[x][y] = 0;
        }
    }

    // Для каждого возможного размера корабля и каждой ориентации
    for (int size = 1; size <= 4; size++) {
        for (auto [x, y] : field_[0]->recentHits_) {
            // Проверить, можно ли разместить корабль горизонтально
            bool free = true;
            for (int i = 0; i < size; ++i) {
                if (!field_[0]->CheckMiss(x + i, y)) {
                    free = false;
                    break;
                }
            }
            if (free) {
                for (int i = 1; i < size; ++i) {
                    probabilityMap[x + i][y] += 10;
                }
            }
            free = true;
            for (int i = 0; i < size; ++i) {
                if (!field_[0]->CheckMiss(x - i, y)) {
                    free = false;
                    break;
                }
            }
            if (free) {
                for (int i = 1; i < size; ++i) {
                    probabilityMap[x - i][y] += 10;
                }
            }

            // Проверить, можно ли разместить корабль вертикально
            free = true;
            for (int i = 0; i < size; ++i) {
                if (!field_[0]->CheckMiss(x, y + i)) {
                    free = false;
                    break;
                }
            }
            if (free) {
                for (int i = 1; i < size; ++i) {
                    probabilityMap[x][y + i] += 10;
                }
            }
            free = true;
            for (int i = 0; i < size; ++i) {
                if (!field_[0]->CheckMiss(x, y - i)) {
                    free = false;
                    break;
                }
            }
            if (free) {
                for (int i = 1; i < size; ++i) {
                    probabilityMap[x][y - i] += 10;
                }
            }
        }
    }

    // Для каждого возможного размера корабля и каждой ориентации
    for (int size = 1; size <= 4; size++) {
        for (int x = 0; x < FIELD_SIZE; ++x) {
            for (int y = 0; y < FIELD_SIZE; ++y) {
                // Проверить, можно ли разместить корабль горизонтально
                bool free = true;
                for (int i = 0; i < size; ++i) {
                    if (!field_[0]->CheckCell(x + i, y)) {
                        free = false;
                        break;
                    }
                }
                if (free) {
                    for (int i = 0; i < size; ++i) {
                        probabilityMap[x + i][y] += 1;
                    }
                }

                // Проверить, можно ли разместить корабль вертикально
                free = true;
                for (int i = 0; i < size; ++i) {
                    if (!field_[0]->CheckCell(x, y + i)) {
                        free = false;
                        break;
                    }
                }
                if (free) {
                    for (int i = 0; i < size; ++i) {
                        probabilityMap[x][y + i] += 1;
                    }
                }
            }
        }
    }
}
