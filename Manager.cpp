#include "Manager.h"
using namespace std;

Manager::Manager(string n, string nat, string l)
    : name(std::move(n)), nationality(std::move(nat)), league(std::move(l)) {imagePath = "images/managers/" + name + ".png";}

const string& Manager::getName() const { return name; }
const string& Manager::getImagePath() const { return imagePath; }
const string& Manager::getNationality() const { return nationality; }
const string& Manager::getLeague() const { return league; }
int Manager::getChemistryBonus(const Player& p) const {
    if (p.getLeague() == league || p.getNationality() == nationality) {
        return 1;
    }
    return 0;
}

ostream& operator<<(ostream& os, const Manager& m) {
    os << "Manager: " << m.name << " (" << m.nationality << ", " << m.league << ")";
    return os;
}