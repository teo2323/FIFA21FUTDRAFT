#pragma once
#include <string>
#include <fstream>
#include <iostream>

class Logger {
    std::ofstream logFile;
    Logger();

public:
    static Logger& getInstance();


    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;

    ~Logger();

    void log(const std::string& message);
};