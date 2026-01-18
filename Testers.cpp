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

    cout << "Player 1 Rating: " << p1.getRating() << "\n";
    cout << "Player 1 Club: " << p1.getClub() << "\n";
    cout << "Player 1 Role: " << p1.getRole() << "\n";

    cout << "Link P1-P2: " << p1.calcLink(p2) << "\n";

    Goalkeeper gk("Donnarumma", "Italy", "Ligue1", "PSG", "GK", "Goalkeeper", 89);
    cout << "GK Penalty on GK: " << gk.getChemistryPenalty("GK") << "\n";
    cout << "GK Penalty on ST: " << gk.getChemistryPenalty("ST") << "\n";

    Attacker att("Messi", "Argentina", "MLS", "Miami", "RW", "Attacker", 93);
    const auto &alts = att.getAltPositions();
    if (!alts.empty()) cout << "Attacker has alt positions.\n";

    auto clonePtr = att.clone();
    cout << "Clone Rating: " << clonePtr->getRating() << "\n";

    cout << "Total players: " << Player::getTotalPlayers() << "\n";
    cout << "--- End Player Tests ---\n";
}

void Testers::runManagerTests() {
    Manager m("Mouricio_Pochetino", "Argentina", "Ligue1");
    Player p("Messi", "Argentina", "LaLiga", "Barcelona", "RW", "Attacker", 93);

    cout << "Chemistry Bonus: " << m.getChemistryBonus(p) << "\n";
    cout << "Manager Name: " << m.getName() << "\n";
    cout << "Player Name: " << p.getName() << "\n";
}

void Testers::runTeamTests() {
    Formation f("433");
    Team t(f);

    auto p = make_unique<Attacker>("Messi", "Argentina", "LaLiga", "Barcelona", "RW", "Starter", 93);
    const Player &pRef = *p;

    t.addPlayer("RW", std::move(p));

    cout << "Team Rating: " << t.computeRating() << "\n";
    cout << "Team Chemistry: " << t.computeChemistry() << "\n";
    cout << "Team Overall: " << t.computeOverall() << "\n";

    if (t.positionTaken("RW")) cout << "RW is taken.\n";
    if (t.isPlayerInTeam(pRef)) cout << "Messi is in team.\n";

    const Player *ptr = t.getPlayerOnPosition("RW");
    if (ptr) cout << "Player at RW: " << ptr->getName() << "\n";
}