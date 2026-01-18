#include "GameApp.h"
#include "DraftSession.h"
#include "UIFactory.h"
#include "Testers.h"
#include "Exception.h"
#include <iostream>
#include <optional>

using namespace std;

GameApp::GameApp() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    window.create(sf::VideoMode({1280, 720}), "FIFA Draft 2025", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);
}

GameApp& GameApp::getInstance() {
    static GameApp instance;
    return instance;
}

struct MenuButton {
    sf::RectangleShape shape;
    sf::Text label;
    std::string formationName;

    MenuButton(const sf::Font &font, string name, float x, float y)
        : shape(UIFactory::createButton({180.0f, 60.0f}, sf::Color(50, 150, 50), {x, y})),
          label(UIFactory::createText(font, name, 24, sf::Color::White, {x, y})),
          formationName(std::move(name)) {

          label.setString(formationName);
    }
};

void GameApp::run() {
    Testers::runPlayerTests();
    Testers::runManagerTests();
    Testers::runTeamTests();

    sf::Font font;
    if (!font.openFromFile("arial.ttf")) throw FileMissingException("arial.ttf");

    map<string, sf::Texture> formationTextures;
    vector<string> formationNames = {"433", "442", "343", "4321", "532"};

    for(const auto& name : formationNames) {
        sf::Texture t;
        if(t.loadFromFile("images/formations/" + name + ".png")) {
            t.setSmooth(true);
            formationTextures[name] = std::move(t);
        }
    }

    vector<MenuButton> buttons;
    float startX = 140.0f;
    float gap = 200.0f;
    for (size_t i = 0; i < formationNames.size(); ++i) {
        buttons.emplace_back(font, formationNames[i], startX + i * gap, 600.0f);
    }

    sf::Text title = UIFactory::createText(font, "FIFA FUT Draft", 40, sf::Color::White, {640, 50});
    sf::Text subtitle = UIFactory::createText(font, "Click Left: Select | Hold Right: Preview", 20, sf::Color(200, 200, 200), {640, 100});

    sf::Texture dummyTex;
    (void)dummyTex.resize({1, 1});
    sf::Sprite previewSprite(dummyTex);

    while (window.isOpen()) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        bool showPreview = false;

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* mp = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mp->button == sf::Mouse::Button::Left) {
                    for (const auto &btn : buttons) {
                        if (btn.shape.getGlobalBounds().contains(mousePos)) {
                            Formation f(btn.formationName);
                            DraftSession session(window, f, scoreStats);
                            session.run();
                            window.setTitle("FIFA Draft 2025");
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(20, 20, 20));
        window.draw(title);
        window.draw(subtitle);

        string bestText = "Best Score: " + to_string(scoreStats.getBest());
        sf::Text hs = UIFactory::createText(font, bestText, 20, sf::Color(255, 215, 0), {640, 680});
        window.draw(hs);

        for (auto &btn : buttons) {
            if (btn.shape.getGlobalBounds().contains(mousePos)) {
                btn.shape.setScale({1.1f, 1.1f});
                btn.shape.setFillColor(sf::Color(70, 200, 70));

                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && formationTextures.count(btn.formationName)) {
                    showPreview = true;
                    previewSprite.setTexture(formationTextures[btn.formationName], true);
                    sf::FloatRect b = previewSprite.getLocalBounds();
                    previewSprite.setOrigin({b.size.x/2, b.size.y/2});
                    previewSprite.setPosition({640, 350});
                }
            } else {
                btn.shape.setScale({1.0f, 1.0f});
                btn.shape.setFillColor(sf::Color(50, 150, 50));
            }
            window.draw(btn.shape);
            window.draw(btn.label);
        }

        if (showPreview) {
            window.draw(previewSprite);
        }

        window.display();
    }
}