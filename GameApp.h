#pragma once
#include <SFML/Graphics.hpp>
#include "SessionStats.h"
#include <memory>

class GameApp {
    sf::RenderWindow window;
    SessionStats<int> scoreStats;
    SessionStats<double> ratingStats;

    GameApp();

public:
    static GameApp& getInstance();
    GameApp(const GameApp&) = delete;
    void operator=(const GameApp&) = delete;

    void run();
};