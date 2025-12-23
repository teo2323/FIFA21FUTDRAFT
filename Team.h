#pragma once
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include "Formation.h"
#include "Player.h"
#include "Manager.h"
#include <memory>

class Team {
    Formation formation;
    std::map<std::string, std::unique_ptr<Player> > players;
    std::vector<std::unique_ptr<Player> > reserves;
    Manager manager;

public:
    void swapPlayers(const std::string &pos1, const std::string &pos2);

    explicit Team(const Formation &f);

    Team(const Team &other);

    Team &operator=(Team other);

    ~Team() = default;

    void addPlayer(const std::string &pos, std::unique_ptr<Player> p);

    void setManager(const Manager &m);

    void addReserve(std::unique_ptr<Player> p);

    void swapReserves(int index1, int index2);

    void swapStarterWithReserve(const std::string &starterPos, int reserveIdx);

    friend void swap(Team &first, Team &second) noexcept;

    [[nodiscard]] bool positionTaken(const std::string &pos) const;

    [[nodiscard]] double computeRating() const;

    [[nodiscard]] int computeChemistry() const;

    [[nodiscard]] double computeOverall() const;

    [[nodiscard]] bool isPlayerInTeam(const Player &p) const;

    [[nodiscard]] Player *getPlayerOnPosition(const std::string &pos) const;

    [[nodiscard]] int getPlayerChemistry(const std::string &pos) const;

    [[nodiscard]] Player *getReserve(int index) const;

    friend std::ostream &operator<<(std::ostream &os, const Team &t);
};
