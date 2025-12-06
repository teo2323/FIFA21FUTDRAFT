#include "Testers.h"
#include "Player.h"
#include "Manager.h"
#include "Team.h"
#include "Formation.h"
#include <iostream>
#include <string>

using namespace std;

void Testers::runPlayerTests() {
    Player p1("Ronaldo", "Portugal", "Juventus", "Manchester_United", "ST", "Attacker", 92);
    Player p2("Mbappe", "France", "Ligue1", "PSG", "ST", "Attacker", 91);

    [[maybe_unused]] int rating1 = p1.getRating();
    [[maybe_unused]] int rating2 = p2.getRating();
    [[maybe_unused]] int link = p1.calcLink(p2);
    
    [[maybe_unused]] string Culoarelink;
    if (link == 0) Culoarelink = "rosu";
    if (link == 1) Culoarelink = "portocaliu";
    if (link == 2) Culoarelink = "galben";
    if (link == 3) Culoarelink = "verde";
    
    [[maybe_unused]] const std::string& n1 = p1.getName();
    [[maybe_unused]] const std::string& n2 = p2.getName();

    cout << "Jucatori de test " << n1 << ", " << n2 << " | Link: " << Culoarelink << "\n";
}

void Testers::runManagerTests() {
    Manager m("Mouricio_Pochetino", "Argentina", "Ligue1");
    Player p("Messi", "Argentina", "LaLiga", "Barcelona", "RW", "Attacker", 93);

    [[maybe_unused]] int chem = m.getChemistryBonus(p);
    [[maybe_unused]] const std::string& Mname = m.getName();
    [[maybe_unused]] const std::string& Pname = p.getName();

    cout << "Test de chemistry intre managerul " << Mname << " Si jucatorul " << Pname << " | Chem: " << chem << "\n";
}

void Testers::runTeamTests() {
    Formation f("433");
    Team t(f);
    Player p("Messi", "Argentina", "LaLiga", "Barcelona", "RW", "Starter", 93);
    t.addPlayer("RW", p);

    [[maybe_unused]] double rating = t.computeRating();
    [[maybe_unused]] int chem = t.computeChemistry();
    [[maybe_unused]] double overall = t.computeOverall();

    cout << "Echipa test: rating=" << rating << " | chem=" << chem << " | overall=" << overall << "\n";
}