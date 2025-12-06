#include "Database.h"
#include <fstream>

using namespace std;

void Database::loadPlayers(const string& filename, const string& positionGroup) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cerr << "Nu s-a putut deschide fisierul: " << filename << "\n";
        return;
    }
    string name, nat, league, club, pos, role;
    int rating;
    while (fin >> name >> nat >> league >> club >> pos >> role >> rating) {
        Player p(name, nat, league, club, pos, role, rating);
        playersByPosition[positionGroup].emplace_back(std::move(p));
    }
    fin.close();
}

void Database::loadAll() {
    // Asigura-te ca fisierele .txt sunt in folderul executabilului (cmake-build-debug)
    loadPlayers("LB.txt", "LB");
    loadPlayers("GK.txt", "GK");
    loadPlayers("CB.txt", "CB");
    loadPlayers("RB.txt", "RB");
    loadPlayers("CM.txt", "CM");
    loadPlayers("LM.txt", "LM");
    loadPlayers("RM.txt", "RM");
    loadPlayers("LW.txt", "LW");
    loadPlayers("RW.txt", "RW");
    loadPlayers("ST.txt", "ST");
}

const vector<Player>& Database::getPlayersByPosition(const string& positionGroup) const {
    static const vector<Player> empty{};
    auto it = playersByPosition.find(positionGroup);
    if (it == playersByPosition.end()) return empty;
    return it->second;
}

ostream& operator<<(ostream& os, const Database& db) {
    os << "     Jucatori incarcati in baza de date    \n";
    if (db.playersByPosition.empty()) {
        os << "Baza de date este goala!\n";
        return os;
    }
    for (const auto& entry : db.playersByPosition) {
        os << "\nPozitie: " << entry.first << " (" << entry.second.size() << " jucatori)\n";
    }
    return os;
}