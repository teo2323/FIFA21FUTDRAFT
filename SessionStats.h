#pragma once
#include <vector>
#include <algorithm>
#include <iostream>

template <typename T>
class SessionStats {
    std::vector<T> history;

public:
    void addEntry(T value) {
        history.push_back(value);

        std::sort(history.begin(), history.end(), [](const T& a, const T& b) {
            return a > b;
        });
    }

    [[nodiscard]] const std::vector<T>& getHistory() const {
        return history;
    }

    [[nodiscard]] T getBest() const {
        if (history.empty()) return T(0);
        return history.front();
    }

    [[nodiscard]] double getAverage() const {
        if (history.empty()) return 0.0;
        T sum = 0;
        for (const auto& val : history) sum += val;
        return static_cast<double>(sum) / history.size();
    }
};