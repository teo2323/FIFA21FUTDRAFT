#include "ChemistryLink.h"
#include <cmath>

ChemistryLink::ChemistryLink(sf::Vector2f p1, sf::Vector2f p2,float thickness) {
    sf::Vector2f diff = p2-p1;
    float lenth = std::sqrt(diff.x*diff.x + diff.y*diff.y);
    float angleVal = std::atan2(diff.y, diff.x)* 180.f / 3.14159265f;

    line.setSize({lenth, thickness});
    line.setOrigin({0.0f, thickness / 2.0f});
    line.setPosition(p1);
    line.setRotation(sf::degrees(angleVal));
}
GreenLink::GreenLink(sf::Vector2f p1,sf::Vector2f p2)
    :ChemistryLink(p1,p2, 6.0f)
{
    line.setFillColor(sf::Color::Green);
}

void GreenLink::draw(sf::RenderWindow& window) const {
    window.draw(line);
}

YellowLink::YellowLink(sf::Vector2f p1, sf::Vector2f p2)
    : ChemistryLink(p1, p2, 4.0f)
{
    line.setFillColor(sf::Color::Yellow);
}

void YellowLink::draw(sf::RenderWindow& window) const {
    window.draw(line);
}

OrangeLink::OrangeLink(sf::Vector2f p1, sf::Vector2f p2)
    : ChemistryLink(p1, p2, 4.0f)
{
    line.setFillColor(sf::Color(255, 165, 0));
}

void OrangeLink::draw(sf::RenderWindow& window) const {
    window.draw(line);
}

RedLink::RedLink(sf::Vector2f p1, sf::Vector2f p2)
    : ChemistryLink(p1, p2, 2.0f)
{
    line.setFillColor(sf::Color(200, 50, 50));
}

void RedLink::draw(sf::RenderWindow& window) const {
    window.draw(line);
}
