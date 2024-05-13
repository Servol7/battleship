#pragma once

#include "field.h"

class Game {
public:
    Game();
    ~Game();
    Field* GetField(size_t i) const;

    bool EndOfGame() const;
    bool Win(size_t i) const;
    void MakeMove1(size_t x, size_t y);
    void MakeMove2();

private:
    Field* field_[2];
    bool move_;
};
