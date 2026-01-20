#include "AnalysisSystem.h"
#include <iomanip>
#include <sstream>
#include <cmath>

using namespace std;

AnalysisSystem::AnalysisSystem(sf::Font& f)
    : font(f), totalSquadValue(0),

      panel(UIFactory::createButton({300.0f, 450.0f}, sf::Color(20, 20, 40, 230), {1100.0f, 385.0f})),
      valueText(UIFactory::createText(f, "Value: $0", 22, sf::Color::Green, {1100.0f, 190.0f})),
      achievementsHeader(UIFactory::createText(f, "ACHIEVEMENTS", 24, sf::Color(255, 215, 0), {1100.0f, 230.0f}))
{
    panel.setOutlineColor(sf::Color(255, 215, 0));
    panel.setOutlineThickness(2.0f);
    initAchievements();

    UIFactory::centerOrigin(achievementsHeader);
}

void AnalysisSystem::initAchievements() {

    achievements.push_back({"Squad Builder", "Finish the draft", false, 100});
    achievements.push_back({"Perfect Chemistry", "Reach 100 Chem", false, 500});
    achievements.push_back({"Galacticos", "Rating 88+", false, 1000});
    achievements.push_back({"Billionaire", "Value > 1B", false, 750});
    achievements.push_back({"The Wall", "GK Rating > 90", false, 250});
}

long long AnalysisSystem::calculatePlayerValue(const Player* p) {
    if (!p) return 0;

    long long base = 500000;
    int rating = p->getRating();

    if (rating > 90) {
        base = 50000000 + (long long)(rating - 90) * 15000000;
    } else if (rating > 85) {
        base = 20000000 + (long long)(rating - 85) * 5000000;
    } else if (rating > 80) {
        base = 5000000 + (long long)(rating - 80) * 1000000;
    } else {
        base += (long long)(rating - 50) * 50000;
    }

    if (p->getRole() == "Icon" || p->getLeague() == "Legends") {
        base *= 2;
    }

    return base;
}

string AnalysisSystem::formatMoney(long long value) {
    stringstream ss;
    if (value >= 1000000) {
        double millions = (double)value / 1000000.0;
        ss << "$" << fixed << setprecision(1) << millions << "M";
    } else if (value >= 1000) {
        double k = (double)value / 1000.0;
        ss << "$" << fixed << setprecision(1) << k << "K";
    } else {
        ss << "$" << value;
    }
    return ss.str();
}

void AnalysisSystem::checkConditions(const Team& team) {
    if (team.computeOverall() > 10) achievements[0].unlocked = true;
    if (team.computeChemistry() >= 100) achievements[1].unlocked = true;
    if (team.computeRating() >= 88) achievements[2].unlocked = true;
    if (totalSquadValue > 1000000000) achievements[3].unlocked = true;

    const Player* gk = team.getPlayerOnPosition("GK");
    if (gk && gk->getRating() > 90) achievements[4].unlocked = true;
}

void AnalysisSystem::analyzeTeam(const Team& team, const Formation& fmt) {
    totalSquadValue = 0;

    for (const auto& pos : fmt.getPositions()) {
        const Player* p = team.getPlayerOnPosition(pos);
        if (p) {
            totalSquadValue += calculatePlayerValue(p);
        }
    }

    for (int i = 0; i < 7; ++i) {
        const Player* p = team.getReserve(i);
        if (p) {
            totalSquadValue += calculatePlayerValue(p) / 2;
        }
    }

    checkConditions(team);

    valueText.setString("Squad Value: " + formatMoney(totalSquadValue));
    UIFactory::centerOrigin(valueText);

    achievementTexts.clear();

    float startY = 270.0f;

    for (const auto& ach : achievements) {

        string status = ach.unlocked ? "[X] " : "[ ] ";
        sf::Color c = ach.unlocked ? sf::Color::Green : sf::Color(180, 180, 180);

        sf::Text tTitle = UIFactory::createText(font, status + ach.title, 18, c, {1100.0f, startY});
        UIFactory::centerOrigin(tTitle);
        achievementTexts.push_back(tTitle);


        startY += 20.0f;

        sf::Color cDesc = sf::Color(150, 150, 150, 200);
        sf::Text tDesc = UIFactory::createText(font, ach.description, 12, cDesc, {1100.0f, startY});
        UIFactory::centerOrigin(tDesc);
        achievementTexts.push_back(tDesc);


        startY += 35.0f;
    }
}

void AnalysisSystem::draw(sf::RenderWindow& window) {
    window.draw(panel);
    window.draw(valueText);
    window.draw(achievementsHeader);

    for (const auto& t : achievementTexts) {
        window.draw(t);
    }
}