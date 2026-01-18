#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class UIFactory {
public:
    static sf::Text createText(const sf::Font& font, const std::string& content, unsigned int size, const sf::Color& color, const sf::Vector2f& pos);
    static sf::RectangleShape createButton(const sf::Vector2f& size, const sf::Color& color, const sf::Vector2f& pos);

    template <typename T>
    static void centerOrigin(T& object) {
        auto bounds = object.getLocalBounds();
        object.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    }
};

