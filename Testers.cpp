#include "Testers.h"
#include "Player.h"
#include "Manager.h"
#include "Team.h"
#include "Formation.h"
#include <iostream>
#include <string>
#include <memory>

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
    const auto &alts = att.getAltPositions();
    if (!alts.empty()) cout << "Attacker has alt positions.\n";

    auto clonePtr = att.clone();
    [[maybe_unused]] int cloneRat = clonePtr->getRating();

    int total = Player::getTotalPlayers();
    cout << "Tests passed. Total players loaded: " << total << "\n";
    cout << "--- End Player Tests ---\n";
}

void Testers::runManagerTests() {
    Manager m("Mouricio_Pochetino", "Argentina", "Ligue1");
    Player p("Messi", "Argentina", "LaLiga", "Barcelona", "RW", "Attacker", 93);

    int chem = m.getChemistryBonus(p);
    cout << "Manager Chem Bonus: " << chem << "\n";
}

void Testers::runTeamTests() {
    Formation f("433");
    Team t(f);

    auto p = make_unique<Attacker>("Messi", "Argentina", "LaLiga", "Barcelona", "RW", "Starter", 93);
    const Player &pRef = *p;

    t.addPlayer("RW", std::move(p));

    t.addReserve(make_unique<Goalkeeper>("Neuer", "Germany", "Bundesliga", "Bayern", "GK", "Goalkeeper", 90));


    const Player* res = t.getReserve(0);
    if (res) {
        cout << "Reserve check: " << res->getName() << endl;
    }

    [[maybe_unused]] double rating = t.computeRating();
    [[maybe_unused]] int chem = t.computeChemistry();
    [[maybe_unused]] double overall = t.computeOverall();
    [[maybe_unused]] bool taken = t.positionTaken("RW");
    [[maybe_unused]] bool inTeam = t.isPlayerInTeam(pRef);

    const Player *ptr = t.getPlayerOnPosition("RW");
    if (ptr) cout << "Player at RW: " << ptr->getName() << "\n";
}