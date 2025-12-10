#include "Team.h"
#include <algorithm>
#include <iomanip>
#include <ranges>

using namespace std;

Team::Team(const Formation& f) : formation(f) {}

void Team::addPlayer(const string& pos, const Player& p) { players[pos] = p; }
void Team::setManager(const Manager& m) { manager = m; }
bool Team::positionTaken(const string& pos) const { return players.contains(pos); }

double Team::computeRating() const {
    int sum = 0;
    for (const auto& player : std::views::values(players))
        sum += player.getRating();
    return players.empty() ? 0.0 : sum / static_cast<double>(players.size());
}

int Team::computeChemistry() const {
    int chemistry = 0;
    for (const auto& kv1 : players) {
        const string pos1 = kv1.first;
        const Player& p1 = kv1.second;
        int localChem = 10; // Base chemistry
        

        
        for (const auto& kv2 : players) {
            const string pos2 = kv2.first;
            const Player& p2 = kv2.second;
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
        int withManager = min(10, max(3, localChem + manager.getChemistryBonus(p1)));
        chemistry += withManager;
    }
    return min(100, chemistry);
}

double Team::computeOverall() const { return computeRating() + computeChemistry(); }

bool Team::isPlayerInTeam(const Player& p) const {
    return ranges::any_of(players | views::values,
                          [&](const Player& player) { return player.getName() == p.getName(); });
}

ostream& operator<<(ostream& os, const Team& t) {
    os << "   Echipa  :\n";
    for (auto& pos : t.formation.getPositions()) {
        os << setw(4) << pos << " -> ";
        auto it = t.players.find(pos);
        if (it != t.players.end()) os << it->second << "\n";
        else os << "(nu ai ales inca)\n";
    }
    os << t.manager << "\n";
    os << "Team Rating: " << t.computeRating() << " | Chemistry: " << t.computeChemistry() << " | Overall: " << t.computeOverall() << "\n";
    return os;
}