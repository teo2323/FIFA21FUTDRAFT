#include <iostream>
#include <limits>
#include "Player.h"
#include "Formation.h"
#include "DraftSession.h"
#include "Testers.h"

using namespace std;

int main() {
    const Player p("Messi","Argentina","LaLiga","PSG","RW","Attacker",93);

    [[maybe_unused]] const std::string& name = p.getName();
    [[maybe_unused]] const std::string& club = p.getClub();
    [[maybe_unused]] const std::string& position = p.getPosition();
    [[maybe_unused]] const std::string& role = p.getRole();

    Testers::runPlayerTests();
    Testers::runManagerTests();
    Testers::runTeamTests();

    cout << "Bun venit in FIFA Draft Demo!\n";
    cout << "Alege formatia:\n 1 -> 4-3-3 \n 2 -> 4-4-2 \n 3 -> 3-4-3 \n 4 -> 4-3-2-1 \n 5 -> 5-3-2 \nOptiunea: ";
    int opt;

    while (true) {
        cin >> opt;
        if (cin.fail() || opt < 1 || opt > 5) {
            cout << "Invalid. Incercati din nou: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else break;
    }
    string formatieAleasa;
    if (opt==1) formatieAleasa = "433";
    else if (opt==2) formatieAleasa = "442";
    else if (opt==3) formatieAleasa = "343";
    else if (opt==4) formatieAleasa = "4321";
    else if (opt==5) formatieAleasa = "532";

    Formation f(formatieAleasa);
    DraftSession draft(f);
    draft.start();

    return 0;
}