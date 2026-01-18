#include "Logger.h"
#include <ctime>

Logger::Logger() {
    logFile.open("game_log.txt", std::ios::app);
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(const std::string& message) {
    std::time_t now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    std::string formattedMsg = "[" + std::string(buf) + "] " + message;


    if (logFile.is_open()) {
        logFile << formattedMsg << std::endl;
    }


    std::cout << formattedMsg << std::endl;
}