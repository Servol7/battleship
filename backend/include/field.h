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
    void Save(std::ostream& out) const;
    void Load(std::istream& in);
    const std::set<std::pair<size_t, size_t>>& GetRecentHits() const { return recentHits_; }
    bool AlreadyHit(size_t x, size_t y) const { return hit_.count({x, y}) > 0; }
    bool CheckCell(int x, int y) const;
    bool CheckMiss(int x, int y) const;

    std::set<std::pair<size_t, size_t>> recentHits_;

private:
    std::vector<Ship> ships_;
    std::set<std::pair<size_t, size_t>> hit_;
    size_t dead_ = 0;
};
