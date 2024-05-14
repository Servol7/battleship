#pragma once

#include "field.h"
#include <random>


enum class Difficulty {
    Easy,
    Medium,
    Hard
};

class Game {
public:
    Game();
    ~Game();
    void SetDifficulty(Difficulty diff);
    Field* GetField(size_t i) const;

    bool EndOfGame() const;
    bool Win(size_t i) const;
    void MakeMove1(size_t x, size_t y);
    void MakeMove2();
    void SaveGame(const std::string& filename);
    void LoadGame(const std::string& filename);
    void UpdateProbabilityMap();

private:
    Field* field_[2];
    bool move_;
    Difficulty difficulty_ = Difficulty::Easy;
    std::vector<std::vector<int>> probabilityMap;
};
