#pragma once
#include <map>
#include <string>
#include <iostream>
#include "Formation.h"
#include "Team.h"
#include "Database.h"

class DraftSession {
    Formation formation;
    Team team;
    Database db;
    std::map<std::string, std::string> positionMap;
public:
    explicit DraftSession(const Formation& f);
    DraftSession(const DraftSession& other) = default;
    DraftSession& operator=(const DraftSession& other) = default;
    ~DraftSession() = default;

    void start();
    friend std::ostream& operator<<(std::ostream& os, const DraftSession& ds);
};