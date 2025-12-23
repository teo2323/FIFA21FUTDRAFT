#pragma once
#include <exception>
#include <string>
#include <utility>


class GameException : public std::exception {
protected:
    std::string message;

public:
    explicit GameException(std::string msg) : message(std::move(msg)) {
    }


    [[nodiscard]] const char *what() const noexcept override {
        return message.c_str();
    }
};


class FileMissingException : public GameException {
public:
    explicit FileMissingException(const std::string &filename)
        : GameException("Eroare critica: Fisierul '" + filename + "' nu a fost gasit!") {
    }
};


class DataCorruptedException : public GameException {
public:
    explicit DataCorruptedException(const std::string &context)
        : GameException("Eroare date: Format invalid in " + context) {
    }
};


class InvalidOperationException : public GameException {
public:
    explicit InvalidOperationException(const std::string &reason)
        : GameException("Operatie ilegala: " + reason) {
    }
};
