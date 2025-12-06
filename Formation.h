#pragma once
#include <string>
#include <vector>
#include <iostream>

class Formation {
    std::string name;
    std::vector<std::string> positions;
    std::vector<std::pair<std::string, std::string>> links;
public:
    explicit Formation(const std::string& n = "");
    Formation(const Formation& other) = default;
    Formation& operator=(const Formation& other) = default;
    ~Formation() = default;

    [[nodiscard]] const std::vector<std::string>& getPositions() const;
    [[nodiscard]] const std::vector<std::pair<std::string, std::string>>& getLinks() const;

    friend std::ostream& operator<<(std::ostream& os, const Formation& f);
};