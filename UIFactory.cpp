#include "UIFactory.h"

sf::Text UIFactory::createText(const sf::Font& font, const std::string& content, unsigned int size, const sf::Color& color, const sf::Vector2f& pos) {
    sf::Text text(font, content, size);
    text.setFillColor(color);

    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({static_cast<float>(static_cast<int>(bounds.size.x / 2)),
                    static_cast<float>(static_cast<int>(bounds.size.y / 2))});
    text.setPosition(pos);
    return text;
}

sf::RectangleShape UIFactory::createButton(const sf::Vector2f& size, const sf::Color& color, const sf::Vector2f& pos) {
    sf::RectangleShape shape(size);
    shape.setFillColor(color);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setOrigin({size.x / 2.0f, size.y / 2.0f});
    shape.setPosition(pos);
    return shape;
}