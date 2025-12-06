#pragma once
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "Player.h"

class Database {
    std::map<std::string, std::vector<Player>> playersByPosition;
public:
    explicit Database() = default;
    Database(const Database& other) = default;
    Database& operator=(const Database& other) = default;
    ~Database() = default;

    void loadPlayers(const std::string& filename, const std::string& positionGroup);
    void loadAll();
    [[nodiscard]] const std::vector<Player>& getPlayersByPosition(const std::string& positionGroup) const;

    friend std::ostream& operator<<(std::ostream& os, const Database& db);
};