#include "Formation.h"
using namespace std;

Formation::Formation(const string& n) : name(n) {
    if (n == "433") {
        positions = {"GK", "LB", "LCB", "RCB", "RB", "LCM", "CDM", "RCM", "LW", "ST", "RW"};
        links = {{"GK", "RCB"}, {"RCB", "RB"}, {"RB", "RCM"}, {"RCM", "RW"}, {"RW", "ST"}, {"ST", "LW"}, {"LW", "LCM"}, {"LCM", "LB"},
                 {"LB", "LCB"}, {"LCB", "RCB"}, {"LCB", "GK"}, {"LCM", "CDM"}, {"CDM", "RCM"}, {"CDM", "ST"}, {"CDM", "LCB"}, {"CDM", "RCB"}};
    } else if (n == "442") {
        positions = {"GK", "LB", "LCB", "RCB", "RB", "LM", "LCM", "RCM", "RM", "LST", "RST"};
        links = {{"GK", "RCB"}, {"RCB", "RB"}, {"RB", "RM"}, {"RM", "RST"}, {"RST", "LST"}, {"LST", "LM"}, {"LM", "LB"}, {"LB", "LCB"},
                 {"LCB", "RCB"}, {"LCB", "GK"}, {"LM", "LCM"}, {"LCM", "RCM"}, {"RCM", "RM"}, {"LST", "LCM"}, {"RST", "RCM"}, {"LCM", "LCB"}, {"RCM", "RCB"}};
    } else if (n == "343") {
        positions = {"GK", "LCB", "CB", "RCB", "LM", "LCM", "RCM", "RM", "LW", "ST", "RW"};
        links = {{"GK", "LCB"}, {"GK", "CB"}, {"GK", "RCB"}, {"RCB", "RM"}, {"RM", "RW"}, {"RW", "ST"}, {"ST", "LW"}, {"LW", "LM"},
                 {"LM", "LCB"}, {"LCB", "CB"}, {"CB", "RCB"}, {"CB", "LCM"}, {"CB", "RCM"}, {"LCM", "LM"}, {"RCM", "RM"}, {"LCM", "RCM"}, {"RCM", "ST"}, {"LCM", "ST"}};
    } else if (n == "4321") {
        positions = {"GK", "LB", "LCB", "RCB", "RB", "LCM", "CDM", "RCM", "LST", "ST", "RST"};
        links = {{"GK", "RCB"}, {"RCB", "RB"}, {"RB", "RCM"}, {"RCM", "RST"}, {"RST", "ST"}, {"ST", "LST"}, {"LST", "LCM"}, {"LCM", "LB"},
                 {"LB", "LCB"}, {"LCB", "RCB"}, {"LCB", "GK"}, {"LCM", "CDM"}, {"CDM", "RCM"}, {"CDM", "LST"}, {"CDM", "RST"}};
    } else if (n == "532") {
        positions = {"GK", "LB", "LCB", "CB", "RCB", "RB", "CDM", "LCM", "RCM", "LST", "RST"};
        links = {{"GK", "LCB"}, {"GK", "CB"}, {"GK", "RCB"}, {"RCB", "RB"}, {"RB", "RCM"}, {"RCM", "RST"}, {"RST", "LST"}, {"LST", "LCM"},
                 {"LCM", "LB"}, {"LB", "LCB"}, {"LCB", "CB"}, {"CB", "RCB"}, {"LCB", "LCM"}, {"RCB", "RCM"}, {"CB", "CDM"}, {"CDM", "LCM"}, {"CDM", "RCM"}, {"CDM", "LST"}, {"CDM", "RST"}};
    }
}

const vector<string>& Formation::getPositions() const { return positions; }
const vector<pair<string, string>>& Formation::getLinks() const { return links; }

ostream& operator<<(ostream& os, const Formation& f) {
    os << "Formation: " << f.name << "\nPositions: \n";
    for (const auto& p : f.positions) os << p << " ";
    os << "\n Links";
    for (const auto& link : f.links)
        os << "  " << link.first << " <-> " << link.second << "\n";
    return os;
}