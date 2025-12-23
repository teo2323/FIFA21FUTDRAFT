#pragma once
#include <string>
#include <iostream>
#include "Player.h"

class Manager {
    std::string name, nationality, league;
    std::string imagePath;

public:
    explicit Manager(std::string n = "", std::string nat = "", std::string l = "");

    Manager(const Manager &other) = default;

    Manager &operator=(const Manager &other) = default;

    ~Manager() = default;

    [[nodiscard]] const std::string &getNationality() const;

    [[nodiscard]] const std::string &getLeague() const;

    [[nodiscard]] const std::string &getImagePath() const;

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] int getChemistryBonus(const Player &p) const;

    friend std::ostream &operator<<(std::ostream &os, const Manager &m);
};
