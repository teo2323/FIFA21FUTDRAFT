#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
#include "Formation.h"
#include "Team.h"
#include "Database.h"
#include "ChemistryLink.h"
#include <memory>
#include "SessionStats.h"
#include "MatchSystem.h"
#include "AnalysisSystem.h"
#include "SoundManager.h"

enum class DraftState {
    DRAFTING,
    CHOOSING_MANAGER,
    FINISHED,
    SUMMARY,
    SIMULATION
};

struct CardOption {
    sf::RectangleShape shape;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Text nameText;
    sf::Text ratingText;

    std::unique_ptr<Player> playerPtr;
    Manager manager;
    bool isManager;

    CardOption(const sf::Font &font, const sf::Texture &dummyTex)
        : sprite(dummyTex), nameText(font), ratingText(font), isManager(false) {
    }

    CardOption(const CardOption&) = delete;
    CardOption& operator=(const CardOption&) = delete;
};


struct SelectedVisual {
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Text info;

    SelectedVisual(const sf::Font &font, sf::Texture texCopy)
        : texture(std::move(texCopy)),
          sprite(texture),
          info(font) {
    }
};

class DraftSession {
    Formation formation;
    Team team;
    Database db;
    std::map<std::string, std::string> positionMap;

    DraftState state;
    SessionStats<int>& globalStats;

    sf::RenderWindow &window;
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


    std::vector<std::unique_ptr<CardOption>> currentOptions;

    std::vector<std::unique_ptr<SelectedVisual> > sidebarVisuals;
    std::vector<std::unique_ptr<SelectedVisual> > reserveVisuals;

    sf::Sprite previewSprite;
    int selectedSwapIndex;
    std::vector<std::unique_ptr<ChemistryLink> > linkLines;

    sf::RectangleShape finishButton;
    sf::Text finishText;
    std::unique_ptr<MatchSystem> matchSystem;
    sf::RectangleShape playMatchButton;
    sf::Text playMatchText;
    std::unique_ptr<AnalysisSystem> analysisSystem;
    bool summaryProcessed;
    bool soundPlayed;
public:
    DraftSession(sf::RenderWindow &win, const Formation &f, SessionStats<int>& stats);

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

    void updateLinksVisuals();

    void handleSwapSelection(int index, bool isReserve);

    void drawSidebar();
    void drawField();
    void drawOverlay();
    void drawSummary();
};