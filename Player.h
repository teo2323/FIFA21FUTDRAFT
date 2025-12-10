#pragma once
#include <string>
#include <iostream>

class Player {
    std::string name, nationality, league, club, position, role,imagePath;
    int rating;
public:
    explicit Player(std::string n="", std::string nat="", std::string l="", std::string c="", std::string pos="", std::string r="", int rate=0);
    Player(const Player& other) = default;
    Player& operator=(const Player& other) = default;
    ~Player() = default;
    [[nodiscard]] const std::string& getImagePath() const;
    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] const std::string& getNationality() const;
    [[nodiscard]] const std::string& getLeague() const;
    [[nodiscard]] const std::string& getClub() const;
    [[nodiscard]] const std::string& getPosition() const;
    [[nodiscard]] const std::string& getRole() const;
    [[nodiscard]] int getRating() const;

    [[nodiscard]] int calcLink(const Player& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Player& p);
};