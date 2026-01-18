#include "MatchSystem.h"
#include <iomanip>
#include <sstream>
#include <iostream>

using namespace std;

MatchSystem::MatchSystem(sf::Font& f, int uOverall)
    : font(f),
      userOverall(uOverall),
      opponentOverall(0),
      opponentName(""),
      gameTime(0.0f),
      userScore(0),
      opponentScore(0),
      matchEnded(false),
      matchStarted(false),
      lastCheckMinute(-1),

      board(UIFactory::createButton({600.0f, 300.0f}, sf::Color(0, 0, 0, 220), {640.0f, 360.0f})),
      scoreText(UIFactory::createText(f, "0 - 0", 60, sf::Color::Yellow, {640.0f, 340.0f})),
      timerText(UIFactory::createText(f, "00:00", 40, sf::Color::Cyan, {640.0f, 420.0f})),
      teamsText(f),
      eventText(UIFactory::createText(f, "Match Starting...", 20, sf::Color(200, 200, 200), {640.0f, 470.0f}))
{
    board.setOutlineColor(sf::Color::White);
    board.setOutlineThickness(3.0f);

    std::random_device rd;
    rng.seed(rd());

    generateOpponent();

    teamsText = UIFactory::createText(font, "MY TEAM  vs  " + opponentName, 30, sf::Color::White, {640.0f, 260.0f});

    UIFactory::centerOrigin(scoreText);
    UIFactory::centerOrigin(timerText);
    UIFactory::centerOrigin(teamsText);
    UIFactory::centerOrigin(eventText);
}

void MatchSystem::generateOpponent() {
    vector<string> names = {"Real Madrid", "Man City", "Bayern", "Liverpool", "PSG", "Inter", "Arsenal", "Barcelona"};
    uniform_int_distribution<> distName(0, names.size() - 1);
    opponentName = names[distName(rng)];


    uniform_int_distribution<> distOvr(-5, 5);
    opponentOverall = userOverall + distOvr(rng);

    if(opponentOverall > 200) opponentOverall = 200;
}

void MatchSystem::startMatch() {
    matchStarted = true;
    matchEnded = false;
    gameTime = 0.0f;
    userScore = 0;
    opponentScore = 0;
    lastCheckMinute = -1;
    frameClock.restart();

    eventText.setString("Match Starting...");
    UIFactory::centerOrigin(eventText);
}

string MatchSystem::formatTime(float t) {
    int minutes = static_cast<int>(t);
    int seconds = static_cast<int>((t - minutes) * 60);

    stringstream ss;
    ss << setfill('0') << setw(2) << minutes << ":" << setw(2) << seconds;
    return ss.str();
}

void MatchSystem::attemptScoring() {

    int diff = userOverall - opponentOverall;


    double baseChance = 0.025;

    uniform_real_distribution<> chance(0.0, 1.0);


    double userProb = baseChance + (diff * 0.002);


    if (userProb < 0.005) userProb = 0.005;

    if (chance(rng) < userProb) {
        userScore++;
        eventText.setString("GOAL FOR MY TEAM!");
        eventText.setFillColor(sf::Color::Green);
        UIFactory::centerOrigin(eventText);
    }

    double oppProb = baseChance - (diff * 0.002);
    if (oppProb < 0.005) oppProb = 0.005;

    if (chance(rng) < oppProb) {
        opponentScore++;
        eventText.setString("GOAL FOR " + opponentName + "!");
        eventText.setFillColor(sf::Color::Red);
        UIFactory::centerOrigin(eventText);
    }
}

void MatchSystem::update() {
    if (!matchStarted || matchEnded) return;

    float dtReal = frameClock.restart().asSeconds();


    float speedFactor = 8.0f;

    gameTime += dtReal * speedFactor;


    int currentMinute = static_cast<int>(gameTime);
    if (currentMinute > lastCheckMinute && currentMinute <= 90) {
        attemptScoring();
        lastCheckMinute = currentMinute;


        if (currentMinute % 5 == 0 && eventText.getString().toAnsiString().find("GOAL") != string::npos) {
             eventText.setString("...");
             eventText.setFillColor(sf::Color(200, 200, 200));
             UIFactory::centerOrigin(eventText);
        }
    }


    if (gameTime >= 90.0f) {
        gameTime = 90.0f;
        matchEnded = true;
        eventText.setString("FULL TIME");
        eventText.setFillColor(sf::Color::White);
        UIFactory::centerOrigin(eventText);
    }

    timerText.setString(formatTime(gameTime));
    scoreText.setString(to_string(userScore) + " - " + to_string(opponentScore));

    UIFactory::centerOrigin(timerText);
    UIFactory::centerOrigin(scoreText);
}

void MatchSystem::draw(sf::RenderWindow& window) {
    window.draw(board);
    window.draw(teamsText);
    window.draw(scoreText);
    window.draw(timerText);
    window.draw(eventText);
}

bool MatchSystem::isFinished() const {
    return matchEnded;
}