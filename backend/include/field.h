#pragma once

#include <set>
#include <string>
#include <vector>

#include "ship.h"

const size_t FIELD_SIZE = 10;

class Field {
public:
    Field(bool generated = true);
    void AddShip(const Ship& ship);
    bool IsValid() const;
    void PrintToCmd() const;
    void GenerateField();
    int MakeHit(int x, int y);
    bool CheckDead() const;
    std::vector<std::string> Prepare(bool hide = true) const;

private:
    std::vector<Ship> ships_;
    std::set<std::pair<size_t, size_t>> hit_;
    size_t dead_ = 0;
};
