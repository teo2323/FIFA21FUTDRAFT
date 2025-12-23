#include "Player.h"
#include <iomanip>

using namespace std;


int Player::totalPlayersLoaded = 0;


Player::Player(string n, string nat, string l, string c, string pos, string r, int rate)
    : name(std::move(n)),
      nationality(std::move(nat)),
      league(std::move(l)),
      club(std::move(c)),
      position(std::move(pos)),
      role(std::move(r)),
      rating(rate) {
    imagePath = "images/players/" + name + ".png";
    totalPlayersLoaded++;
}


const string &Player::getImagePath() const { return imagePath; }
const string &Player::getName() const { return name; }
const string &Player::getNationality() const { return nationality; }
const string &Player::getLeague() const { return league; }
const string &Player::getClub() const { return club; }
const string &Player::getPosition() const { return position; }
const string &Player::getRole() const { return role; }
int Player::getRating() const { return rating; }
const std::set<std::string> &Player::getAltPositions() const { return alternativePositions; }
int Player::getTotalPlayers() { return totalPlayersLoaded; }


unique_ptr<Player> Player::clone() const { return make_unique<Player>(*this); }

int Player::getChemistryPenalty(const string &currentSlot) const {
    if (currentSlot == position) return 0;
    return -5;
}

int Player::calcLink(const Player &other) const {
    if (league == "Legends" && other.league != "Legends") {
        if (other.nationality == nationality) {
            return 2;
        } else {
            return 1;
        }
    }
    if (league == other.league && club == other.club && nationality == other.nationality) return 3;
    if ((league == other.league && nationality == other.nationality) || (
            club == other.club && nationality != other.nationality) || (
            nationality == other.nationality && other.league == "Legends"))
        return 2;
    if (league == other.league || nationality == other.nationality || other.league == "Legends") return 1;
    return 0;
}

void Player::print(ostream &os) const {
    os << setw(15) << left << name << " | "
            << setw(5) << position << " | "
            << setw(3) << rating;
}

ostream &operator<<(ostream &os, const Player &p) {
    p.print(os);
    return os;
}


unique_ptr<Player> Goalkeeper::clone() const { return make_unique<Goalkeeper>(*this); }

int Goalkeeper::getChemistryPenalty(const string &currentSlot) const {
    if (currentSlot == "GK") return 0;
    return -5;
}


Defender::Defender(string n, string nat, string l, string c, string pos, string r, int rate)
    : Player(std::move(n), std::move(nat), std::move(l), std::move(c), std::move(pos), std::move(r), rate) {
    alternativePositions = {"RB", "LB", "CB", "LCB", "RCB"};
}

unique_ptr<Player> Defender::clone() const { return make_unique<Defender>(*this); }

int Defender::getChemistryPenalty(const string &currentSlot) const {
    bool exact = (currentSlot == position);
    bool isCentral = (position == "CB" && (currentSlot == "LCB" || currentSlot == "RCB"));
    if (exact || isCentral) return 0;

    if (alternativePositions.contains(currentSlot)) return -2;
    return -5;
}


Midfielder::Midfielder(string n, string nat, string l, string c, string pos, string r, int rate)
    : Player(std::move(n), std::move(nat), std::move(l), std::move(c), std::move(pos), std::move(r), rate) {
    alternativePositions = {"LM", "RM", "CDM", "LCM", "RCM"};
}

unique_ptr<Player> Midfielder::clone() const { return make_unique<Midfielder>(*this); }

int Midfielder::getChemistryPenalty(const string &currentSlot) const {
    bool exact = (currentSlot == position);
    bool isCentral = (position == "CM" && (currentSlot == "LCM" || currentSlot == "RCM" || currentSlot == "CDM"));
    if (exact || isCentral) return 0;
    if (alternativePositions.contains(currentSlot)) return -2;
    return -5;
}


Attacker::Attacker(string n, string nat, string l, string c, string pos, string r, int rate)
    : Player(std::move(n), std::move(nat), std::move(l), std::move(c), std::move(pos), std::move(r), rate) {
    alternativePositions = {"ST", "LW", "RW", "RST", "LST"};
}

unique_ptr<Player> Attacker::clone() const { return make_unique<Attacker>(*this); }

int Attacker::getChemistryPenalty(const string &currentSlot) const {
    bool exact = (currentSlot == position);
    bool isStriker = (position == "ST" && (currentSlot == "LST" || currentSlot == "RST"));
    if (exact || isStriker) return 0;
    if (alternativePositions.contains(currentSlot)) return -2;
    return -5;
}

Icon::Icon(string n, string nat, string l, string c, string pos, string r, int rate)
    : Player(std::move(n), std::move(nat), std::move(l), std::move(c), std::move(pos), std::move(r), rate) {
}

unique_ptr<Player> Icon::clone() const {
    return make_unique<Icon>(*this);
}

int Icon::getChemistryPenalty(const string &currentSlot) const {
    bool exact = (currentSlot == position);
    if (exact) return 0;
    return 0;
}
