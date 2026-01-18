#include "GameApp.h"
#include "Logger.h"
#include <iostream>

int main() {

    Logger::getInstance().log("--- Application Started ---");

    try {
        GameApp::getInstance().run();
    } catch (const std::exception &e) {

        Logger::getInstance().log("[CRITICAL ERROR]: " + std::string(e.what()));
        return -1;
    } catch (...) {
        Logger::getInstance().log("[UNKNOWN ERROR] An unexpected error occurred.");
        return -2;
    }


    Logger::getInstance().log("--- Application Closed Normally ---");
    return 0;
}