#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <iomanip>

class Player {
    std::string name, nationality, league, club, position, role;
    int rating;
public:
    explicit Player(std::string n="", std::string nat="", std::string l="", std::string c="", std::string pos="", std::string r="", int rate=0)
    : name(std::move(n)),
      nationality(std::move(nat)),
      league(std::move(l)),
      club(std::move(c)),
      position(std::move(pos)),
      role(std::move(r)),
      rating(rate) {}

    Player(const Player& other);

    Player& operator=(const Player& other) = default;

    ~Player();

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

std::ostream& operator<<(std::ostream& os, const Player& p);

#endif
