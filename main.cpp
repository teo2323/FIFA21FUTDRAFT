#include <iostream>
#include <vector>
#include <map>
#include <optional> // Necesar pentru SFML 3.0
#include <SFML/Graphics.hpp>
#include "DraftSession.h"
#include "Formation.h"


struct MenuButton {
    sf::RectangleShape shape;
    sf::Text label;
    std::string formationName;

    explicit MenuButton(const sf::Font& font) : label(font) {}
};

int main() {

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "FIFA Draft - OOP Project", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);


    sf::Font font;
    if (!font.openFromFile("arial.ttf")) {
        std::cerr << "Eroare critica: Nu am gasit arial.ttf! Verifica folderul cmake-build-debug.\n";
        return -1;
    }


    std::map<std::string, sf::Texture> formationTextures;
    std::vector<std::string> formationNames = {"433", "442", "343", "4321", "532"};

    for (const auto& name : formationNames) {
        std::string path = "images/formations/" + name + ".png";
        sf::Texture tex;
        tex.setSmooth(true);
        if (tex.loadFromFile(path)) {
            formationTextures[name] = std::move(tex);
        } else {
            std::cerr << "Atentie: Lipseste imaginea " << path << "\n";
        }
    }


    sf::Texture dummyTexture;
    sf::Sprite previewSprite(dummyTexture);


    std::vector<MenuButton> menuButtons;
    float startX = 140.0f;
    float gap = 20.0f;
    float btnWidth = 180.0f;
    float btnHeight = 60.0f;

    for (size_t i = 0; i < formationNames.size(); ++i) {
        MenuButton btn(font);
        btn.formationName = formationNames[i];

        btn.shape.setSize({btnWidth, btnHeight});
        btn.shape.setFillColor(sf::Color(50, 150, 50));
        btn.shape.setOutlineColor(sf::Color::White);
        btn.shape.setOutlineThickness(2);
        btn.shape.setOrigin({btnWidth / 2, btnHeight / 2});

        float posX = startX + static_cast<float>(i) * (btnWidth + gap) + (btnWidth / 2);
        btn.shape.setPosition({posX, 600.0f});

        btn.label.setString(formationNames[i]);
        btn.label.setCharacterSize(24);
        sf::FloatRect textRect = btn.label.getLocalBounds();
        btn.label.setOrigin({textRect.size.x / 2.0f, textRect.size.y / 2.0f});
        btn.label.setPosition(btn.shape.getPosition());

        menuButtons.push_back(btn);
    }


    sf::Text title(font, "FIFA FUT Draft", 40);
    sf::FloatRect tRect = title.getLocalBounds();
    title.setOrigin({tRect.size.x / 2, tRect.size.y / 2});
    title.setPosition({640, 50});

    sf::Text subtitle(font, "Click STANGA: Selectare | Tine CLICK DREAPTA: Preview", 20);
    subtitle.setFillColor(sf::Color(200, 200, 200));
    sf::FloatRect sRect = subtitle.getLocalBounds();
    subtitle.setOrigin({sRect.size.x / 2, sRect.size.y / 2});
    subtitle.setPosition({640, 100});



    while (window.isOpen()) {


        while (const std::optional event = window.pollEvent()) {


            if (event->is<sf::Event::Closed>()) {
                window.close();
            }


            else if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePress->button == sf::Mouse::Button::Left) {

                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    for (const auto& btn : menuButtons) {
                        if (btn.shape.getGlobalBounds().contains(mousePos)) {
                            std::cout << "--> Formatie aleasa: " << btn.formationName << ". Incepe Draftul...\n";

                            Formation chosenFormation(btn.formationName);
                            DraftSession session(window, chosenFormation);
                            session.run();


                            window.setTitle("FIFA Draft - OOP Project");
                        }
                    }
                }
            }
        }


        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        bool showPreview = false;

        for (auto& btn : menuButtons) {
            if (btn.shape.getGlobalBounds().contains(mousePos)) {
                btn.shape.setScale({1.1f, 1.1f});
                btn.shape.setFillColor(sf::Color(70, 200, 70));


                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
                    if (formationTextures.count(btn.formationName)) {
                        showPreview = true;
                        previewSprite.setTexture(formationTextures[btn.formationName], true);

                        sf::FloatRect bounds = previewSprite.getLocalBounds();
                        previewSprite.setOrigin({bounds.size.x / 2, bounds.size.y / 2});
                        previewSprite.setPosition({640, 320});
                    }
                }
            } else {
                btn.shape.setScale({1.0f, 1.0f});
                btn.shape.setFillColor(sf::Color(50, 150, 50));
            }
        }


        window.clear(sf::Color(20, 20, 20));

        window.draw(title);
        window.draw(subtitle);

        for (const auto& btn : menuButtons) {
            window.draw(btn.shape);
            window.draw(btn.label);
        }

        if (showPreview) {
            sf::RectangleShape overlay(sf::Vector2f(1280, 720));
            overlay.setFillColor(sf::Color(0, 0, 0, 200));
            window.draw(overlay);
            window.draw(previewSprite);
        }

        window.display();
    }

    return 0;
}