#include "player.hpp"
#include <string>

// Player::Player(std::string n="", std::string nat="", std::string l="", std::string c="", std::string pos="", std::string r="", int rate=0)
//     : name(std::move(n)),
//       nationality(std::move(nat)),
//       league(std::move(l)),
//       club(std::move(c)),
//       position(std::move(pos)),
//       role(std::move(r)),
//       rating(rate) {}
Player::Player(const Player& other) = default;

//Player::Player& operator=(const Player& other) = default;

Player::~Player() = default;

[[nodiscard]] const std::string& Player::getName() const { return name; }
[[nodiscard]] const std::string& Player::getNationality() const { return nationality; }
[[nodiscard]] const std::string& Player::getLeague() const { return league; }
[[nodiscard]] const std::string& Player::getClub() const { return club; }
[[nodiscard]] const std::string& Player::getPosition() const { return position; }
[[nodiscard]] const std::string& Player::getRole() const { return role; }
[[nodiscard]] int Player::getRating() const { return rating; }

[[nodiscard]] int Player::calcLink(const Player& other) const {
    if (league==other.league && club==other.club && nationality==other.nationality) return 3; // verde
    if ((league==other.league && nationality==other.nationality) || (club==other.club && nationality!=other.nationality)) return 2; // galben
    if (league==other.league || nationality==other.nationality) return 1; // portocaliu
    return 0; // rosu
}

std::ostream& operator<<(std::ostream& os, const Player& p) {
    os << std::setw(15) << std::left << p.name
       << " | " << std::setw(12) << p.nationality
       << " | " << std::setw(12) << p.league
       << " | " << std::setw(10) << p.club
       << " | " << std::setw(5) << p.position
       << " | " << std::setw(3) << p.rating;
    return os;
}

