#include "Team.h"
#include <algorithm>
#include <iomanip>
#include <ranges>

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

int Team::computeChemistry() const {
    int chemistry = 0;
    for (const auto& kv1 : players) {
        const string pos1 = kv1.first;
        const Player& p1 = *kv1.second;
        int localChem = 10;

        localChem += p1.getChemistryPenalty(pos1);

        for (const auto& kv2 : players) {
            const string pos2 = kv2.first;
            const Player& p2 = *kv2.second;
            if (pos1 == pos2) continue;
            
            for (const auto& link : formation.getLinks()) {
                if ((link.first == pos1 && link.second == pos2) || (link.second == pos1 && link.first == pos2)) {
                    int linkType = p1.calcLink(p2);
                    if (linkType == 0) localChem -= 3;
                    else if (linkType == 2) localChem += 3;
                    else if (linkType == 3) localChem = 10; 
                }
            }
        }
        localChem = max(0, min(10, localChem));
        int withManager = min(10, max(3, localChem + manager.getChemistryBonus(p1)));
        chemistry += withManager;
    }
    return min(100, chemistry);
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