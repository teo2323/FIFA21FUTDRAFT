#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <random>
#include "UIFactory.h"

class MatchSystem {
    sf::Font& font;
    int userOverall;
    int opponentOverall;
    std::string opponentName;

    float gameTime;
    int userScore;
    int opponentScore;

    bool matchEnded;
    bool matchStarted;

    int lastCheckMinute;

    sf::RectangleShape board;
    sf::Text scoreText;
    sf::Text timerText;
    sf::Text teamsText;
    sf::Text eventText;

    sf::Clock frameClock;

    std::mt19937 rng;

public:
    MatchSystem(sf::Font& f, int uOverall);

    MatchSystem(const MatchSystem&) = delete;
    MatchSystem& operator=(const MatchSystem&) = delete;

    void startMatch();
    void update();
    void draw(sf::RenderWindow& window);
    bool isFinished() const;

private:
    void generateOpponent();
    void attemptScoring();
    std::string formatTime(float t);
};