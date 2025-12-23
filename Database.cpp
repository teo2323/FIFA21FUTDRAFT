#include "Database.h"
#include "Exception.h"
#include <fstream>
#include <memory>
#include <sstream>

using namespace std;

void Database::loadPlayers(const string &filename, const string &positionGroup) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        throw FileMissingException(filename);
    }

    string line;
    int lineNumber = 0;

    while (getline(fin, line)) {
        lineNumber++;
        if (line.empty()) continue;

        stringstream ss(line);
        string name, nat, league, club, pos, role;
        int rating;

        if (!(ss >> name >> nat >> league >> club >> pos >> role >> rating)) {
            throw DataCorruptedException(filename + " la linia " + to_string(lineNumber));
        }

        unique_ptr<Player> newPlayer;

        if (role == "Goalkeeper") {
            newPlayer = make_unique<Goalkeeper>(name, nat, league, club, pos, role, rating);
        } else if (role == "Defender") {
            newPlayer = make_unique<Defender>(name, nat, league, club, pos, role, rating);
        } else if (role == "Midfielder") {
            newPlayer = make_unique<Midfielder>(name, nat, league, club, pos, role, rating);
        } else if (role == "Attacker") {
            newPlayer = make_unique<Attacker>(name, nat, league, club, pos, role, rating);
        } else {
            newPlayer = make_unique<Player>(name, nat, league, club, pos, role, rating);
        }

        playersByPosition[positionGroup].push_back(std::move(newPlayer));
    }
    fin.close();
}

void Database::loadManagers(const string &filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        throw FileMissingException(filename);
    }

    string line;
    int lineNumber = 0;

    while (getline(fin, line)) {
        lineNumber++;
        if (line.empty()) continue;

        stringstream ss(line);
        string name, nat, league;

        if (!(ss >> name >> nat >> league)) {
            throw DataCorruptedException(filename + " la linia " + to_string(lineNumber));
        }

        managers.emplace_back(name, nat, league);
    }
    fin.close();
}

void Database::loadAll() {
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
    loadManagers("MANAGER.txt");
}

const vector<Manager> &Database::getManagers() const {
    return managers;
}

const vector<unique_ptr<Player> > &Database::getPlayersByPosition(const string &positionGroup) const {
    static const vector<unique_ptr<Player> > empty{};
    auto it = playersByPosition.find(positionGroup);
    if (it == playersByPosition.end()) return empty;
    return it->second;
}

ostream &operator<<(ostream &os, const Database &db) {
    os << "     Jucatori incarcati in baza de date    \n";
    if (db.playersByPosition.empty()) {
        os << "Baza de date este goala!\n";
        return os;
    }
    for (const auto &entry: db.playersByPosition) {
        os << "\nPozitie: " << entry.first << " (" << entry.second.size() << " jucatori)\n";
    }
    return os;
}
