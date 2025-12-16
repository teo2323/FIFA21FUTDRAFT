#include "Formation.h"
using namespace std;

Formation::Formation(const string& n) : name(n) {


    if (n == "433") {
        positions = {"GK", "LB", "LCB", "RCB", "RB", "LCM", "CDM", "RCM", "LW", "ST", "RW"};
        coordinates = {
            {855, 600},
            {535, 500}, {690, 530}, {1015, 530}, {1180, 500}, // LB, LCB, RCB, RB
            {650, 320}, {855, 340}, {1055, 320},
            {600, 110}, {855, 95},  {1105, 110}
        };
        links = {{"GK", "RCB"}, {"RCB", "RB"}, {"RB", "RCM"}, {"RCM", "RW"}, {"RW", "ST"}, {"ST", "LW"}, {"LW", "LCM"}, {"LCM", "LB"},
                 {"LB", "LCB"}, {"LCB", "RCB"}, {"LCB", "GK"}, {"LCM", "CDM"}, {"CDM", "RCM"}, {"CDM", "ST"}, {"CDM", "LCB"}, {"CDM", "RCB"}};

    } else if (n == "442") {
        positions = {"GK", "LB", "LCB", "RCB", "RB", "LM", "LCM", "RCM", "RM", "LST", "RST"};
        coordinates = {
            {855, 600}, // GK
            {530, 500}, {700, 530}, {1000, 530}, {1180, 500},
            {530, 280}, {712, 330}, {995, 330}, {1180, 280},
            {730, 110}, {970, 110}
        };
        links = {{"GK", "RCB"}, {"RCB", "RB"}, {"RB", "RM"}, {"RM", "RST"}, {"RST", "LST"}, {"LST", "LM"}, {"LM", "LB"}, {"LB", "LCB"},
                 {"LCB", "RCB"}, {"LCB", "GK"}, {"LM", "LCM"}, {"LCM", "RCM"}, {"RCM", "RM"}, {"LST", "LCM"}, {"RST", "RCM"}, {"LCM", "LCB"}, {"RCM", "RCB"}};

    } else if (n == "343") {
        positions = {"GK", "LCB", "CB", "RCB", "LM", "LCM", "RCM", "RM", "LW", "ST", "RW"};
        coordinates = {
            {845, 630}, // GK
            {585, 440}, {845, 440}, {1100, 440},
            {510, 270}, {710, 300}, {980, 300}, {1180, 270},
            {615, 110}, {845, 95}, {1065, 110}
        };
        links = {{"GK", "LCB"}, {"GK", "CB"}, {"GK", "RCB"}, {"RCB", "RM"}, {"RM", "RW"}, {"RW", "ST"}, {"ST", "LW"}, {"LW", "LM"},
                 {"LM", "LCB"}, {"LCB", "CB"}, {"CB", "RCB"}, {"CB", "LCM"}, {"CB", "RCM"}, {"LCM", "LM"}, {"RCM", "RM"}, {"LCM", "RCM"}, {"RCM", "ST"}, {"LCM", "ST"}};

    } else if (n == "4321") {
        positions = {"GK", "LB", "LCB", "RCB", "RB", "LCM", "CDM", "RCM", "LST", "ST", "RST"};
        coordinates = {
            {850, 600},
            {500, 500}, {685, 530}, {1010, 530}, {1190, 500},
            {630, 315}, {845, 315}, {1060, 315},
            {705, 130}, {850, 100},  {990, 130}
        };
        links = {{"GK", "RCB"}, {"RCB", "RB"}, {"RB", "RCM"}, {"RCM", "RST"}, {"RST", "ST"}, {"ST", "LST"}, {"LST", "LCM"}, {"LCM", "LB"},
                 {"LB", "LCB"}, {"LCB", "RCB"}, {"LCB", "GK"}, {"LCM", "CDM"}, {"CDM", "RCM"}, {"CDM", "LST"}, {"CDM", "RST"}};

    } else if (n == "532") {
        positions = {"GK", "LB", "LCB", "CB", "RCB", "RB", "CDM", "LCM", "RCM", "LST", "RST"};
        coordinates = {
            {845, 620}, // GK
            {500, 350}, {620, 430}, {845, 430}, {1055, 430}, {1180, 350},
            {730, 100}, {950, 100}
        };
        links = {{"GK", "LCB"}, {"GK", "CB"}, {"GK", "RCB"}, {"RCB", "RB"}, {"RB", "RCM"}, {"RCM", "RST"}, {"RST", "LST"}, {"LST", "LCM"},
                 {"LCM", "LB"}, {"LB", "LCB"}, {"LCB", "CB"}, {"CB", "RCB"}, {"LCB", "LCM"}, {"RCB", "RCM"}, {"CB", "CDM"}, {"CDM", "LCM"}, {"CDM", "RCM"}, {"CDM", "LST"}, {"CDM", "RST"}};
    }
}
const string& Formation::getName() const{return name;}
const vector<string>& Formation::getPositions() const { return positions; }
const vector<sf::Vector2f>& Formation::getCoordinates() const { return coordinates; }
const vector<pair<string, string>>& Formation::getLinks() const { return links; }

ostream& operator<<(ostream& os, const Formation& f) {
    os << "Formation: " << f.name << "\n";
    return os;
}