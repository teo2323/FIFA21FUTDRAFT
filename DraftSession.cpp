#include "DraftSession.h"
#include <limits>
#include <vector>

using namespace std;

DraftSession::DraftSession(const Formation& f)
    : formation(f),
      team(f),

      positionMap{
              {"GK","GK"},{"LB","LB"},{"LCB","CB"},{"RCB","CB"},{"RB","RB"},
              {"LCM","CM"},{"CDM","CM"},{"RCM","CM"},{"LM","LM"},{"RM","RM"},
              {"LW","LW"},{"RW","RW"},{"ST","ST"},{"LST","ST"},{"RST","ST"},
              {"CB","CB"}
      }
{

}

void DraftSession::start() {
    cout << "Incepe Draft-ul pentru formatia " << endl << formation << "\n";
    cout << "-------------------------------------------\n";

    db.loadAll();

    for (const auto& pos : formation.getPositions()) {
        while (team.positionTaken(pos)) {
            cout << pos << " deja ales, selecteaza alta pozitie.\n";
            break;
        }

        string fileGroup = positionMap[pos];
        vector<Player> options = db.getPlayersByPosition(fileGroup);

        cout << "\nAlegeti jucatorul pentru pozitia " << pos << ":\n";
        for (long long unsigned int i = 0; i < options.size(); i++)
            cout << i + 1 << " - " << options[i] << "\n";

        int opt;
        while (true) {
            cin >> opt;
            if (cin.fail() || opt < 1 || opt > static_cast<int>(options.size()) || team.isPlayerInTeam(options[opt - 1])) {
                cout << "Input invalid sau jucator existent. Incercati din nou: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else break;
        }
        team.addPlayer(pos, options[opt - 1]);
        cout << "\nEchipa momentan:\n" << team << "\n";
    }

    // Aici am adaugat o initializare manuala pt manageri, ca in codul tau
    Manager manager1("Jurgen_Klopp", "Germany", "PremierLeague");
    Manager manager2("Pep_Guardiola", "Spain", "PremierLeague");
    Manager manager3("Mauricio_Pochettino", "Argentina", "Ligue1");
    Manager manager4("Hansi_Flick", "Germany", "Bundesliga");
    Manager manager5("Gigi_Becali", "Romania", "Superliga1");

    vector<Manager> managers = {manager1, manager2, manager3, manager4, manager5};

    cout << "Alegeti managerul:\n";
    for (long long unsigned int i = 0; i < managers.size(); i++) cout << i + 1 << " - " << managers[i] << "\n";
    int mOpt;
    while (true) {
        cin >> mOpt;
        if (cin.fail() || mOpt < 1 || mOpt > static_cast<int>(managers.size())) {
            cout << "Input invalid. Incercati din nou: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else break;
    }
    team.setManager(managers[mOpt - 1]);

    cout << "Draft terminat!\n";
    cout << team << "\n";
}

ostream& operator<<(ostream& os, const DraftSession& ds) {
    os << "     Sesiune draft inceputa      \n";
    os << ds.formation << "\n\n" << ds.team << "\n\n" << ds.db << "\n";
    return os;
}