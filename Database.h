#pragma once
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "Player.h"
#include "Manager.h"
class Database {
    std::map<std::string, std::vector<Player>> playersByPosition;
    std::vector<Manager> managers;
public:
    explicit Database() = default;
    Database(const Database& other) = default;
    Database& operator=(const Database& other) = default;
    ~Database() = default;

    void loadManagers(const std::string& filename);
    void loadPlayers(const std::string& filename, const std::string& positionGroup);
    void loadAll();
    [[nodiscard]] const std::vector<Player>& getPlayersByPosition(const std::string& positionGroup) const;
    [[nodiscard]] const std::vector<Manager>& getManagers() const;
    friend std::ostream& operator<<(std::ostream& os, const Database& db);
};