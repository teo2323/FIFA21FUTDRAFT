#include "Player.h"
#include <iomanip>

using namespace std;

Player::Player(string n, string nat, string l, string c, string pos, string r, int rate)
    : name(std::move(n)), nationality(std::move(nat)), league(std::move(l)),
      club(std::move(c)), position(std::move(pos)), role(std::move(r)), rating(rate) {}

const string& Player::getName() const { return name; }
const string& Player::getNationality() const { return nationality; }
const string& Player::getLeague() const { return league; }
const string& Player::getClub() const { return club; }
const string& Player::getPosition() const { return position; }
const string& Player::getRole() const { return role; }
int Player::getRating() const { return rating; }

int Player::calcLink(const Player& other) const {
    if (league == other.league && club == other.club && nationality == other.nationality) return 3;
    if ((league == other.league && nationality == other.nationality) || (club == other.club && nationality != other.nationality)) return 2;
    if (league == other.league || nationality == other.nationality) return 1;
    return 0;
}

ostream& operator<<(ostream& os, const Player& p) {
    os << setw(15) << left << p.name
       << " | " << setw(12) << p.nationality
       << " | " << setw(12) << p.league
       << " | " << setw(10) << p.club
       << " | " << setw(5) << p.position
       << " | " << setw(3) << p.rating;
    return os;
}