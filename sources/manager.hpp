#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <string>
#include <iostream>
#include "player.hpp"

class Manager {
    std::string name, nationality, league;
public:
    explicit Manager(std::string n = "", std::string nat = "", std::string l = "")
    : name(std::move(n)), nationality(std::move(nat)), league(std::move(l)) {}

    Manager(const Manager& other);

    Manager& operator=(const Manager& other) = default;

    ~Manager();

    [[nodiscard]] const std::string& getName()const;
    [[nodiscard]] const std::string& getNationality() const;
    [[nodiscard]] const std::string& getLeague() const;

    [[nodiscard]] int getChemistryBonus(const Player& p) const;

    friend std::ostream& operator<<(std::ostream& os, const Manager& m);
};

std::ostream& operator<<(std::ostream& os, const Manager& m);

#endif