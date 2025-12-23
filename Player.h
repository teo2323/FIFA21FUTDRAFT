#pragma once
#include <string>
#include <iostream>
#include <set>
#include <memory>

class Player {
protected:
    std::string name, nationality, league, club, position, role, imagePath;
    int rating;

    std::set<std::string> alternativePositions;
    static int totalPlayersLoaded;

public:
    explicit Player(std::string n = "", std::string nat = "", std::string l = "", std::string c = "",
                    std::string pos = "", std::string r = "", int rate = 0);


    Player(const Player &other) = default;

    Player &operator=(const Player &other) = default;


    virtual ~Player() = default;


    [[nodiscard]] virtual std::unique_ptr<Player> clone() const;

    [[nodiscard]] virtual int getChemistryPenalty(const std::string &currentSlot) const;

    virtual void print(std::ostream &os) const;

    [[nodiscard]] const std::string &getImagePath() const;

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] const std::string &getNationality() const;

    [[nodiscard]] const std::string &getLeague() const;

    [[nodiscard]] const std::string &getClub() const;

    [[nodiscard]] const std::string &getPosition() const;

    [[nodiscard]] const std::string &getRole() const;

    [[nodiscard]] int getRating() const;

    [[nodiscard]] const std::set<std::string> &getAltPositions() const;

    [[nodiscard]] int calcLink(const Player &other) const;

    static int getTotalPlayers();

    friend std::ostream &operator<<(std::ostream &os, const Player &p);
};


class Goalkeeper : public Player {
public:
    using Player::Player;

    [[nodiscard]] std::unique_ptr<Player> clone() const override;

    [[nodiscard]] int getChemistryPenalty(const std::string &currentSlot) const override;
};

class Defender : public Player {
public:
    using Player::Player;

    Defender(std::string n, std::string nat, std::string l, std::string c, std::string pos, std::string r, int rate);

    [[nodiscard]] std::unique_ptr<Player> clone() const override;

    [[nodiscard]] int getChemistryPenalty(const std::string &currentSlot) const override;
};

class Midfielder : public Player {
public:
    using Player::Player;

    Midfielder(std::string n, std::string nat, std::string l, std::string c, std::string pos, std::string r, int rate);

    [[nodiscard]] std::unique_ptr<Player> clone() const override;

    [[nodiscard]] int getChemistryPenalty(const std::string &currentSlot) const override;
};

class Attacker : public Player {
public:
    using Player::Player;

    Attacker(std::string n, std::string nat, std::string l, std::string c, std::string pos, std::string r, int rate);

    [[nodiscard]] std::unique_ptr<Player> clone() const override;

    [[nodiscard]] int getChemistryPenalty(const std::string &currentSlot) const override;
};

class Icon : public Player {
public:
    using Player::Player;

    Icon(std::string n, std::string nat, std::string l, std::string c, std::string pos, std::string r, int rate);

    [[nodiscard]] std::unique_ptr<Player> clone() const override;

    [[nodiscard]] int getChemistryPenalty(const std::string &currentSlot) const override;
};
