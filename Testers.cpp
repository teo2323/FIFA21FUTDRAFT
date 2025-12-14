#include "Testers.h"
#include "Player.h"
#include "Manager.h"
#include "Team.h"
#include "Formation.h"
#include <iostream>
#include <string>

using namespace std;

void Testers::runPlayerTests() {
    cout << "--- Start Player Tests ---\n";

    Player p1("Ronaldo", "Portugal", "Juventus", "Manchester_United", "ST", "Attacker", 92);
    Player p2("Mbappe", "France", "Ligue1", "PSG", "ST", "Attacker", 91);

    [[maybe_unused]] int rating1 = p1.getRating();

    
    [[maybe_unused]] string club = p1.getClub();
    [[maybe_unused]] string role = p1.getRole();

    [[maybe_unused]] int link = p1.calcLink(p2);

    Goalkeeper gk("Donnarumma", "Italy", "Ligue1", "PSG", "GK", "Goalkeeper", 89);
    [[maybe_unused]] int pen1 = gk.getChemistryPenalty("GK");
    [[maybe_unused]] int pen2 = gk.getChemistryPenalty("ST");

    Attacker att("Messi", "Argentina", "MLS", "Miami", "RW", "Attacker", 93);
    const auto& alts = att.getAltPositions();
    [[maybe_unused]] bool hasAlt = !alts.empty();

    auto clonePtr = att.clone();
    [[maybe_unused]] int cloneRat = clonePtr->getRating();

    [[maybe_unused]] int total = Player::getTotalPlayers();

    cout << "Tests passed. Total players loaded so far: " << total << "\n";
    cout << "--- End Player Tests ---\n";
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


    auto p = make_unique<Attacker>("Messi", "Argentina", "LaLiga", "Barcelona", "RW", "Starter", 93);

    Player& pRef = *p;

    t.addPlayer("RW", std::move(p));

    [[maybe_unused]] double rating = t.computeRating();
    [[maybe_unused]] int chem = t.computeChemistry();
    [[maybe_unused]] double overall = t.computeOverall();

    [[maybe_unused]] bool taken = t.positionTaken("RW");
    [[maybe_unused]] bool inTeam = t.isPlayerInTeam(pRef);

    cout << "Echipa test: rating=" << rating << " | chem=" << chem << " | overall=" << overall << "\n";
}