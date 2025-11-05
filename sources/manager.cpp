#include <string>
#include <iostream>
#include "manager.hpp"

// Manager::Manager(std::string n = "", std::string nat = "", std::string l = "")
//     : name(std::move(n)), nationality(std::move(nat)), league(std::move(l)) {}

Manager::Manager(const Manager& other) = default;

//Manager::Manager& operator=(const Manager& other) = default;

Manager::~Manager() = default;

[[nodiscard]] const std::string& Manager::getName() const { return name; }
[[nodiscard]] const std::string& Manager::getNationality() const { return nationality; }
[[nodiscard]] const std::string& Manager::getLeague() const { return league; }

[[nodiscard]] int Manager::getChemistryBonus(const Player& p) const {
    if (p.getLeague()==league || p.getNationality()==nationality) {
        return 1;
    }else return 0;
}

std::ostream& operator<<(std::ostream& os, const Manager& m) {
    os << "Manager: " << m.name << " (" << m.nationality << ", " << m.league << ")";
    return os;
}