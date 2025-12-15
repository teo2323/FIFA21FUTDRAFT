#include "Team.h"
#include <algorithm>
#include <iomanip>
#include "Exception.h"
using namespace std;

Team::Team(const Formation& f) : formation(f) {}

Team::Team(const Team& other) : formation(other.formation), manager(other.manager) {
    for (const auto& pair : other.players) {
        players[pair.first] = pair.second->clone();
    }
}
Team& Team::operator=(const Team& other) {
    if (this != &other) {
        formation = other.formation;
        manager = other.manager;
        players.clear();
        for (const auto& pair : other.players) {
            players[pair.first] = pair.second->clone();
        }
    }
    return *this;
}

void Team::addPlayer(const string& pos, unique_ptr<Player> p) {
    players[pos] = std::move(p);
}
void Team::setManager(const Manager& m) { manager = m; }
bool Team::positionTaken(const string& pos) const { return players.contains(pos); }

double Team::computeRating() const {
    int sum = 0;
    if (players.empty()) return 0.0;
    for (const auto& pair : players) {
        sum += pair.second->getRating();
    }
    return sum / static_cast<double>(players.size());
}


int Team::getPlayerChemistry(const std::string& pos) const {
    if (!players.contains(pos)) return 0;

    const Player& p = *players.at(pos);
    int localChem = 10;

    localChem += p.getChemistryPenalty(pos);

    for (const auto& otherPair : players) {
        const string& otherPos = otherPair.first;
        const Player& otherP = *otherPair.second;

        if (pos == otherPos) continue;

        for (const auto& link : formation.getLinks()) {
            if ((link.first == pos && link.second == otherPos) ||
                (link.second == pos && link.first == otherPos)) {

                int linkType = p.calcLink(otherP);
                if (linkType == 0) localChem -= 3;
                else if (linkType == 2) localChem += 3;
                else if (linkType == 3) localChem = 10;
                }
        }
    }

    localChem = max(0, min(10, localChem));

    if (manager.getName() != "") {
        localChem = min(10, localChem + manager.getChemistryBonus(p));
    }

    return localChem;
}
int Team::computeChemistry() const {
    int totalChemistry = 0;

    for (const auto& pair : players) {

        totalChemistry += getPlayerChemistry(pair.first);
    }

    return min(100, totalChemistry);
}

double Team::computeOverall() const { return computeRating() + computeChemistry(); }

bool Team::isPlayerInTeam(const Player& p) const {
    for (const auto& pair : players) {
        if (pair.second->getName() == p.getName()) return true;
    }
    return false;
}

Player* Team::getPlayerOnPosition(const std::string& pos) const {
    auto it = players.find(pos);
    if (it != players.end()) {
        return it->second.get();
    }
    return nullptr;
}

void Team::swapPlayers(const string& pos1, const string& pos2) {

    if (!players.contains(pos1) || !players.contains(pos2)) {
        throw InvalidOperationException("Nu se poate face swap: Unul dintre sloturi este gol!");
    }


   const Player* p1 = players[pos1].get();
   const  Player* p2 = players[pos2].get();




    if (p1->getRole() == "Goalkeeper" && pos2 != "GK") {
        throw InvalidOperationException("Portarii nu pot juca pe pozitii din teren!");
    }
    if (p2->getRole() == "Goalkeeper" && pos1 != "GK") {
        throw InvalidOperationException("Portarii nu pot juca pe pozitii din teren!");
    }


    std::swap(players[pos1], players[pos2]);
}

ostream& operator<<(ostream& os, const Team& t) {
    os << "   Echipa  :\n";
    for (auto& pos : t.formation.getPositions()) {
        os << setw(4) << pos << " -> ";
        auto it = t.players.find(pos);
        if (it != t.players.end()) os << *(it->second) << "\n";
        else os << "(gol)\n";
    }
    os << t.manager << "\n";
    os << "Stats: " << t.computeRating() << " / " << t.computeChemistry() << "\n";
    return os;
}