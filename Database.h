#pragma once
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "Player.h"
#include "Manager.h"

class Database {
    std::map<std::string, std::vector<std::unique_ptr<Player> > > playersByPosition;
    std::vector<Manager> managers;

public:
    explicit Database() = default;

    Database(const Database &other) = delete;

    Database &operator=(const Database &other) = delete;

    ~Database() = default;

    void loadManagers(const std::string &filename);

    void loadPlayers(const std::string &filename, const std::string &positionGroup);

    void loadAll();

    [[nodiscard]] const std::vector<std::unique_ptr<Player> > &getPlayersByPosition(
        const std::string &positionGroup) const;

    [[nodiscard]] const std::vector<Manager> &getManagers() const;

    friend std::ostream &operator<<(std::ostream &os, const Database &db);
};
