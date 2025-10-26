#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
using namespace std;

class Player {
    string name, nationality, league, club, position, role;
    int rating;
public:
    explicit Player(string n="", string nat="", string l="", string c="", string pos="", string r="", int rate=0)
        : name(n), nationality(nat), league(l), club(c), position(pos), role(r), rating(rate) {}

    Player(const Player& other) {
        name = other.name;
        nationality = other.nationality;
        league = other.league;
        club = other.club;
        position = other.position;
        role = other.role;
        rating = other.rating;
    }
    Player& operator=(const Player& other) {
        name = other.name;
        nationality = other.nationality;
        league = other.league;
        club = other.club;
        position = other.position;
        role = other.role;
        rating = other.rating;
        return *this;
    }
    ~Player() {}

    string getName() const { return name; }
    string getNationality() const { return nationality; }
    string getLeague() const { return league; }
    string getClub() const { return club; }
    string getPosition() const { return position; }
    string getRole() const { return role; }
    int getRating() const { return rating; }

    int calcLink(const Player& other) const {
        if (league==other.league && club==other.club && nationality==other.nationality) return 3; // verde
        if ((league==other.league && nationality==other.nationality) || (club==other.club && nationality!=other.nationality)) return 2; // galben
        if (league==other.league || nationality==other.nationality) return 1; // portocaliu
        return 0; // rosu
    }

    friend ostream& operator<<(ostream& os, const Player& p) {
        os << setw(15) << left << p.name
           << " | " << setw(12) << p.nationality
           << " | " << setw(12) << p.league
           << " | " << setw(10) << p.club
           << " | " << setw(5) << p.position
           << " | " << setw(3) << p.rating;
        return os;
    }
};

class Manager {
    string name, nationality, league;
public:
    explicit Manager(string n="", string nat="", string l="")
        : name(n), nationality(nat), league(l) {}

    Manager(const Manager& other) {
        name = other.name;
        nationality = other.nationality;
        league = other.league;
    }

    Manager& operator=(const Manager& other) {
        name = other.name;
        nationality = other.nationality;
        league = other.league;
        return *this;
    }

    ~Manager() {}

    string getName() const { return name; }
    string getNationality() const { return nationality; }
    string getLeague() const { return league; }

    int getChemistryBonus(const Player& p) const {
        if (p.getLeague()==league || p.getNationality()==nationality) {
            return 1;
        }else return 0;
    }

    friend ostream& operator<<(ostream& os, const Manager& m) {
        os << "Manager: " << m.name << " (" << m.nationality << ", " << m.league << ")";
        return os;
    }
};

class Formation {
    string name;
    vector<string> positions;
    vector<pair<string,string>> links;
public:
    explicit Formation(string n="") : name(n) {
        if (n=="433") {
            positions={"GK","LB","LCB","RCB","RB","LCM","CDM","RCM","LW","ST","RW"};
            links={{"GK","RCB"},{"RCB","RB"},{"RB","RCM"},{"RCM","RW"},{"RW","ST"},{"ST","LW"},{"LW","LCM"},{"LCM","LB"},
                   {"LB","LCB"},{"LCB","RCB"},{"LCB","GK"},{"LCM","CDM"},{"CDM","RCM"},{"CDM","ST"},{"CDM","LCB"},{"CDM","RCB"}};
        } else if (n=="442") {
            positions={"GK","LB","LCB","RCB","RB","LM","LCM","RCM","RM","LST","RST"};
            links={{"GK","RCB"},{"RCB","RB"},{"RB","RM"},{"RM","RST"},{"RST","LST"},{"LST","LM"},{"LM","LB"},{"LB","LCB"},
                   {"LCB","RCB"},{"LCB","GK"},{"LM","LCM"},{"LCM","RCM"},{"RCM","RM"},{"LST","LCM"},{"RST","RCM"},{"LCM","LCB"},{"RCM","RCB"}};
        }
    }
    Formation(const Formation& other) {
        name = other.name;
        positions = other.positions;
        links = other.links;
    }

    Formation& operator=(const Formation& other) {
        name = other.name;
        positions = other.positions;
        links = other.links;
        return *this;
    }

    ~Formation() {}

    const vector<string>& getPositions() const { return positions; }
    const vector<pair<string,string>>& getLinks() const { return links; }

    friend ostream& operator<<(ostream& os, const Formation& f) {
        os << "Formation: " << f.name << "\nPositions: ";
        for(auto& p:f.positions) os<<p<<" ";
        return os;
    }
};


int main() {
    cout << "Bun venit in FIFA 21 Draft-Demo!\n";
    cout << "Alege formatia:\n1 - 4-3-3\n2 - 4-4-2\nOptiunea: ";
    int opt;
    cin >> opt;

    string formatieAleasa = (opt == 1) ? "433" : "442";
    Formation f(formatieAleasa);
    cout<<f;


    return 0;
}
