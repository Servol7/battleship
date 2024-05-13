#pragma once

#include <cstdio>
#include <utility>

using std::size_t;

class Ship {
public:
    Ship(size_t size, size_t x, size_t y, bool ornt);
    Ship(size_t size, const std::pair<size_t, size_t>& crd, bool ornt);
    size_t GetSize() const;
    size_t GetX() const;
    size_t GetY() const;
    std::pair<size_t, size_t> GetCrd() const;
    bool GetOrientation() const;
    void AddHit();
    bool CheckDead() const;

private:
    size_t size_;
    std::pair<size_t, size_t> crd_;
    bool ornt_;
    size_t hits_ = 0;
};
