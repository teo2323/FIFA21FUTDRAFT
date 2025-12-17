#pragma once
#include <SFML\Graphics.hpp>

class ChemistryLink {
protected:
    sf::RectangleShape line;
public:
    ChemistryLink(sf::Vector2f p1, sf::Vector2f p2, float thickness);
    virtual ~ChemistryLink() = default;
    virtual void draw(sf::RenderWindow& window) const = 0;
};
class GreenLink: public ChemistryLink {
public:
    GreenLink(sf::Vector2f p1, sf::Vector2f p2);
    void draw(sf::RenderWindow& window) const override;

};
class YellowLink: public ChemistryLink {
    public:
    YellowLink(sf::Vector2f p1, sf::Vector2f p2);
    void draw(sf::RenderWindow& window) const override;
};
class OrangeLink: public ChemistryLink {
    public:
    OrangeLink(sf::Vector2f p1, sf::Vector2f p2);
    void draw(sf::RenderWindow& window) const override;
};
class RedLink: public ChemistryLink {
    public:
    RedLink(sf::Vector2f p1, sf::Vector2f p2);
    void draw(sf::RenderWindow& window) const override;
};