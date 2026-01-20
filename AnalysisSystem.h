#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Team.h"
#include "Formation.h"
#include "UIFactory.h"

struct Achievement {
    std::string title;
    std::string description;
    bool unlocked;
    int pointValue;
};

class AnalysisSystem {
    sf::Font& font;
    long long totalSquadValue;
    std::vector<Achievement> achievements;

    sf::RectangleShape panel;
    sf::Text valueText;
    sf::Text achievementsHeader;
    std::vector<sf::Text> achievementTexts;

public:
    AnalysisSystem(sf::Font& f);

    void analyzeTeam(const Team& team, const Formation& fmt);
    void draw(sf::RenderWindow& window);

private:
    void initAchievements();
    long long calculatePlayerValue(const Player* p);
    std::string formatMoney(long long value);
    void checkConditions(const Team& team);
};