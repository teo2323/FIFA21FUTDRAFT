#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <map>
#include <ranges>
using namespace std;

class Player {
    string name, nationality, league, club, position, role;
    int rating;
public:
    explicit Player(string n="", string nat="", string l="", string c="", string pos="", string r="", int rate=0)
    : name(std::move(n)),
      nationality(std::move(nat)),
      league(std::move(l)),
      club(std::move(c)),
      position(std::move(pos)),
      role(std::move(r)),
      rating(rate) {}


    Player(const Player& other) = default;

    Player& operator=(const Player& other) = default;

    ~Player() = default;

    [[nodiscard]] const string& getName() const { return name; }
    [[nodiscard]] const string& getNationality() const { return nationality; }
    [[nodiscard]] const string& getLeague() const { return league; }
   [[nodiscard]] const string& getClub() const { return club; }
   [[nodiscard]] const string& getPosition() const { return position; }
    [[nodiscard]] const string& getRole() const { return role; }
    [[nodiscard]] int getRating() const { return rating; }

    [[nodiscard]] int calcLink(const Player& other) const {
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
    explicit Manager(string n = "", string nat = "", string l = "")
    : name(std::move(n)), nationality(std::move(nat)), league(std::move(l)) {}

    Manager(const Manager& other) = default;

    Manager& operator=(const Manager& other) = default;

    ~Manager() = default;

    [[nodiscard]] const string& getName() const { return name; }
    [[nodiscard]] const string& getNationality() const { return nationality; }
    [[nodiscard]] const string& getLeague() const { return league; }

    [[nodiscard]] int getChemistryBonus(const Player& p) const {
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
    explicit Formation(const std::string& n = "") : name(n) {
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
    Formation(const Formation& other) =default;

    Formation& operator=(const Formation& other) =default;

    ~Formation() = default;

    [[nodiscard]] const vector<string>& getPositions() const { return positions; }
    [[nodiscard]] const vector<pair<string,string>>& getLinks() const { return links; }

    friend ostream& operator<<(ostream& os, const Formation& f) {
        os << "Formation: " << f.name << "\nPositions: ";
        for(const auto& p:f.positions) os<<p<<" ";
        os << "Links";
        for (const auto& link : f.links)
            os << "  " << link.first << " <-> " << link.second << "\n";
        return os;
    }
};

class Team {
    Formation formation;
    map<string, Player> players;
    Manager manager;
public:
    explicit Team(const Formation& f) : formation(f) {}
    Team(const Team& other) = default;
    Team& operator=(const Team& other) =default;
    ~Team() =default;
    void addPlayer(const string& pos, const Player& p){players[pos]=p;}
    void setManager(const Manager& m){manager=m;}
    [[nodiscard]] bool positionTaken(const string& pos) const{return players.contains(pos);}
    [[nodiscard]] double computeRating() const {
        double sum = 0.0;
        for (const auto& player : std::views::values(players))
            sum += player.getRating();
        return players.empty() ? 0.0 : sum / static_cast<double>(players.size());
    }

    [[nodiscard]] int computeChemistry() const{
        int chemistry=0;
        for(const auto& kv1:players){
            const string pos1=kv1.first;
            const Player& p1=kv1.second;
            int localChem=10;
            for(const auto& kv2:players){
                const string pos2=kv2.first;
                const Player& p2=kv2.second;
                if(pos1==pos2) continue;
                for(const auto& link:formation.getLinks()){
                    if((link.first==pos1 && link.second==pos2)||(link.second==pos1 && link.first==pos2)){
                        int linkType=p1.calcLink(p2);
                        if(linkType==0) localChem-=3;
                        else if(linkType==2) localChem+=3;
                        else if(linkType==3) localChem=10;
                    }
                }
            }
            int withManager=min(10,max(3,localChem+manager.getChemistryBonus(p1)));
            chemistry+=withManager;
        }
        return min(100,chemistry);
    }
    [[nodiscard]] double computeOverall() const{return computeRating()+computeChemistry();}
    friend ostream& operator<<(ostream& os, const Team& t){
        os<<"   Echipa  :\n";
        for(auto& pos:t.formation.getPositions()){
            os<<setw(4)<<pos<<" -> ";
            auto it=t.players.find(pos);
            if(it!=t.players.end()) os<<it->second<<"\n";
            else os<<"(nu ai ales inca)\n";
        }
        os<<t.manager<<"\n";
        os<<"Team Rating: "<<t.computeRating()<<" | Chemistry: "<<t.computeChemistry()<<" | Overall: "<<t.computeOverall()<<"\n";
        return os;
    }
};

class Database {
    map<string, vector<Player>> playersByPosition;
public:
    explicit Database() = default;
    Database(const Database& other) = default;
    Database& operator=(const Database& other) = default;
    ~Database() = default;
    void loadPlayers(const string& filename,const string& positionGroup){
        ifstream fin(filename);
        if(!fin.is_open()){cerr<<"Nu s-a putut deschide fisierul: "<<filename<<"\n"; return;}
        string name,nat,league,club,pos,role; int rating;
        while(fin>>name>>nat>>league>>club>>pos>>role>>rating){
            Player p(name,nat,league,club,pos,role,rating);
            playersByPosition[positionGroup].emplace_back(std::move(p));
        }
        fin.close();

    }
    void loadAll(){
        loadPlayers("LB.txt","LB");
        loadPlayers("GK.txt","GK");
        loadPlayers("CB.txt","CB");
        loadPlayers("RB.txt","RB");
        loadPlayers("CM.txt","CM");
        loadPlayers("LM.txt","LM");
        loadPlayers("RM.txt","RM");
        loadPlayers("LW.txt","LW");
        loadPlayers("RW.txt","RW");
        loadPlayers("ST.txt","ST");
    }
    [[nodiscard]] const vector<Player>& getPlayersByPosition(const string& positionGroup) const{
        static constexpr vector<Player> empty{};
        auto it=playersByPosition.find(positionGroup);
        if(it==playersByPosition.end()) return empty;
        return it->second;
    }
    friend ostream& operator<<(ostream& os, const Database& db) {
        os << "     Jucatori incarcati in baza de date    \n";
        if (db.playersByPosition.empty()) {
            os << "Baza de date este goala!\n";
            return os;
        }

        for (const auto& entry : db.playersByPosition) {
            const string& positionGroup = entry.first;
            const vector<Player>& players = entry.second;
            os << "\nPozitie: " << positionGroup << " (" << players.size() << " jucatori)\n";
            if (players.empty()) {
                os << "   (niciun jucator incarcat)\n";
                continue;
            }
            for (const auto& p : players)
                os << "   " << p << "\n";
        }

        return os;
    }

};

class DraftSession {
    Formation formation;
    Team team;
    Database db;
    map<string,string> positionMap = {
        {"GK","GK"},{"LB","LB"},{"LCB","CB"},{"RCB","CB"},{"RB","RB"},
        {"LCM","CM"},{"CDM","CM"},{"RCM","CM"},{"LM","LM"},{"RM","RM"},
        {"LW","LW"},{"RW","RW"},{"ST","ST"},{"LST","ST"},{"RST","ST"}
    };
public:
    explicit DraftSession(const Formation& f):formation(f),team(f){}
    DraftSession(const DraftSession& other) = default;


    DraftSession& operator=(const DraftSession& other) = default;
    ~DraftSession() = default;
    void start(){
        cout<<"Incepe Draft-ul pentru formatia "<<endl<<formation<<"\n";
        cout<<"-------------------------------------------\n";

        db.loadAll();

        for(const auto& pos:formation.getPositions()){
            while(team.positionTaken(pos)){
                cout<<pos<<" deja ales, selecteaza alta pozitie.\n";
                break;
            }

            string fileGroup=positionMap[pos];
            vector<Player> options=db.getPlayersByPosition(fileGroup);

            cout<<"\nAlegeti jucatorul pentru pozitia "<<pos<<":\n";
            for(long long unsigned int i=0;i<options.size();i++)
                cout<<i+1<<" - "<<options[i]<<"\n";

            int opt; cin>>opt;
            team.addPlayer(pos,options[opt-1]);

            cout<<"\nEchipa momentan:\n"<<team<<"\n";
        }

        Manager manager1("Jurgen_Klopp","Germania","PremierLeague");
        Manager manager2("Pep_Guardiola","Spania","PremierLeague");
        vector<Manager> managers={manager1,manager2};

        cout<<"Alegeti managerul:\n";
        for(long long unsigned int i=0;i<managers.size();i++) cout<<i+1<<" - "<<managers[i]<<"\n";
        int mOpt; cin>>mOpt;
        team.setManager(managers[mOpt-1]);

        cout<<"Draft terminat!\n";
        cout<<team<<"\n";

    }
    friend ostream& operator<<(ostream& os, const DraftSession& ds) {
        os << "     Sesiune draft inceputa      \n";
        os << ds.formation << "\n\n";
        os << ds.team << "\n";
        os << "\n";
        os << ds.db << "\n";
        return os;
    }

};

int main() {
    const Player p("Messi","Argentina","LaLiga","PSG","RW","Starter",93);


    [[maybe_unused]] const std::string& name = p.getName();
    [[maybe_unused]] const std::string& club = p.getClub();
    [[maybe_unused]] const std::string& position = p.getPosition();
    [[maybe_unused]] const std::string& role = p.getRole();


    cout << "Bun venit in FIFA Draft Demo!\n";
    cout << "Alege formatia:\n1 - 4-3-3\n2 - 4-4-2\nOptiunea: ";
    int opt;
    cin >> opt;

    string formatieAleasa = (opt == 1) ? "433" : "442";
    Formation f(formatieAleasa);
    DraftSession draft(f);
    draft.start();

    return 0;
}
