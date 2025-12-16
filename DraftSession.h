#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
#include "Formation.h"
#include "Team.h"
#include "Database.h"


struct CardOption {
    sf::RectangleShape shape;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Text nameText;
    sf::Text ratingText;

    std::unique_ptr<Player> playerPtr;
    Manager manager;
    bool isManager;

    CardOption(const sf::Font& font, const sf::Texture& dummyTex)
        : sprite(dummyTex), nameText(font), ratingText(font), isManager(false) {}
};


struct SelectedVisual {
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Text info;


    SelectedVisual(const sf::Font& font, sf::Texture texCopy)
        : texture(std::move(texCopy)),
          sprite(texture),
          info(font) {}
};

class DraftSession {
    Formation formation;
    Team team;
    Database db;
    std::map<std::string, std::string> positionMap;


    sf::RenderWindow& window;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture defaultCardTexture;
    sf::Texture defaultManagerTexture;
    sf::RectangleShape sidebar;

    sf::Texture dummyTexture;
    sf::Text ratingDisplay;
    sf::Text chemistryDisplay;
    sf::Text overallDisplay;
    sf::RectangleShape statsBackground;


    int currentPositionIndex;
    bool isDrafting;
    bool draftCompleted;
    bool choosingManager;
    std::vector<CardOption> currentOptions;
    std::vector<SelectedVisual> sidebarVisuals;

    sf::Sprite previewSprite;
    int selectedSwapIndex;
public:
    DraftSession(sf::RenderWindow& win, const Formation& f);
    ~DraftSession() = default;
    void run();

private:
    void draw();
    void handleInput();
    void loadResources();
    void generateOptions();
    void generateManagerOptions();
    void selectPlayer(int index);
    void updateStatsUI();
};